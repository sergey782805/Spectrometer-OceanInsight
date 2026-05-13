#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_model {nullptr}, m_spectrometer {nullptr}, m_spectrumProcessor{nullptr}
{
    ui.setupUi(this);

    m_model = new GraphModel();
    m_spectrometer = new Spectrometer();
    m_spectrumProcessor = new SpectrumProcessor();

    ui.quickWidget->engine()->rootContext()->setContextProperty("myModel", m_model);
  
    ui.quickWidget->setSource(QUrl("qrc:/Spectr/Graph.qml"));

    //QObject::connect(ui.pushButton, &QPushButton::clicked, this, &Spectr::updateGraph);
    QObject::connect(ui.averageValue, &QSpinBox::editingFinished, this, &MainWindow::changeAverage);
    QObject::connect(ui.integrationTimeValue, &QSpinBox::editingFinished, this, &MainWindow::changeIntegrationTime);
    QObject::connect(ui.readDark_button, &QPushButton::clicked, this, &MainWindow::readDark);
    QObject::connect(ui.readCorrectedSpectrum_button, &QPushButton::clicked, this, &MainWindow::readCorrectedSpectrum);
    QObject::connect(ui.actionSave_As, &QAction::triggered, this, &MainWindow::saveAs);
    QObject::connect(ui.actionSave_As_Relative, &QAction::triggered, this, &MainWindow::saveAsRelative);
    QObject::connect(ui.actionOpen_calibration_file, &QAction::triggered, this, &MainWindow::openCalibration);
    resize(800, 600);
}
MainWindow::~MainWindow()
{
    delete m_spectrumProcessor;
    m_spectrumProcessor = nullptr;
    delete m_spectrometer;
    m_spectrometer = nullptr;
    delete m_model;
    m_model = nullptr;
}

void MainWindow::readDark()
{
    //std::vector<double> wavelengths = m_spectrometer->readWaveLengths();
    //std::vector<double> darkSpectrum = m_spectrometer->readDarkSpectrum();
    //m_spectrometer->readBiasDarkSpectrum();
    //ui.textBrowser->append("<b style='color: green'>  BIAS Dark Spectrum read</b>");
    m_spectrometer->readCalibDarkSpectrum();
    ui.textBrowser->append("<b style='color: green'>  CALIB Dark Spectrum read</b>");
    //std::vector<double> darkRelative = m_spectrumProcessor->toRelative(darkSpectrum);
    //darkSpectrum = darkRelative;
    changeIntegrationTime();
    //QList<QPointF> data = m_spectrumProcessor->toQList(wavelengths, darkRelative);
    //m_model->setData(data);
    
    //ui.textBrowser->append("<b style='color: green'> Dark Spectrum read</b>");

}
void MainWindow::readCorrectedSpectrum()
{
    std::vector<double> wavelengths = m_spectrometer->readWaveLengths();
    std::vector<double> correctedSpectrum = m_spectrometer->readCorrectedSpectrum();
    wavelengths = {179, 380,  400, 450, 485, 500, 565, 590, 625, 780, 900, 1058 };
    correctedSpectrum = { 50, 70, 20, 30, 40, 10, 15, 1, 80, 50, 11, 15 };
    switch (ui.filter->currentIndex())
    {
    case 1: // savitzkyGolayFilter9
        correctedSpectrum = m_spectrumProcessor->savitzkyGolayFilter9(correctedSpectrum);
        break;
    default: // no filter

        break;
    }

    std::vector<double> relativeCorrectedSpectrum = m_spectrumProcessor->toRelative(correctedSpectrum);
    
    QList<QPointF> data = m_spectrumProcessor->toQList(wavelengths, relativeCorrectedSpectrum);
    m_model->setData(data);
    
    std::vector<double> calibratedSpectrum{};
    calibratedSpectrum = m_spectrumProcessor->calibrate(wavelengths, correctedSpectrum);
    double pfd{m_spectrumProcessor->PARsum(calibratedSpectrum, 350.0, 799.82)};
    double ppfd{ m_spectrumProcessor->PARsum(calibratedSpectrum, 400.069 ,699.978) };

    ui.textBrowser->append("<b style='color: green'> Corrected Spectrum read</b>");
    ui.textBrowser->append("<b style='color: red'> integration time should be set to: </b>" + QString::number(m_spectrometer->detectIntegrationTime()));
    ui.textBrowser->append("<b style='color: orange'> PFD: </b>" + QString::number(pfd));
    ui.textBrowser->append("<b style='color: orange'> PPFD: </b>" + QString::number(ppfd));
}

void MainWindow::changeAverage()
{
    m_spectrometer->setAverageFactor(ui.averageValue->value());
    //ui.textBrowser->append("Min integration time: " + QString::number(m_spectrometr->getMinIntegrationTime()));
    //ui.textBrowser->append("Max integration time: " + QString::number(m_spectrometr->getMaxIntegrationTime()));
    //ui.textBrowser->append("Current integration time: " + QString::number(m_spectrometr->getIntegrationTime()));
    //ui.textBrowser->append("Average values now is : " + QString::number(ui.averageValue->value()));
}
void MainWindow::changeIntegrationTime()
{

	const unsigned long newIntegrationTime{ static_cast<unsigned long> (ui.integrationTimeValue->value())};
    
    const std::vector<double> biasDarkSpectrum{ m_spectrometer->getBiasDarkSpectrum() };
    const std::vector<double> pureDarkSpectrum{ m_spectrometer->getPureDarkSpectrum() };

    if (!biasDarkSpectrum.empty() && !pureDarkSpectrum.empty())
    {
       
        const std::vector<double> adjustedDarkSpectrum{ m_spectrumProcessor->adjustDarkSpectrum(biasDarkSpectrum, pureDarkSpectrum, 0, newIntegrationTime) };

        m_spectrometer->setDarkSpectrum(adjustedDarkSpectrum);

        //m_spectrometer->setDarkIntegrationTime(newIntegrationTime);
    }
	
    m_spectrometer->setIntegrationTime(ui.integrationTimeValue->value());
    //ui.textBrowser->append("Integration time now is: " + QString::number(ui.integrationTimeValue->value()));
}

void MainWindow::saveAs()
{
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Save spectrum"), 
        desktopPath + "/Spectrum1.csv", 
        tr("Text files (*.txt, *.csv);; Any file(*)")
    );

    // Call to save file function with fileName
    //ui.textBrowser->append("<b style='color: orange'> Save As clicked</b>");
    //ui.textBrowser->append("<b style='color: orange'> Dir is: </b>" + fileName);
    QFile savedFile(fileName);
    if (!savedFile.open(QIODevice::NewOnly | QIODevice::Text))
    {
        //return
    }

    auto nm{ m_spectrometer->getLastWavelengths()};
    auto spectrum{ m_spectrometer->getLastSpectrum()};
    
    std::size_t s{ nm.size() };

    QTextStream out(&savedFile);// out FROM programm
    out << "nm,counts\n";
    for (std::size_t i{ 0 }; i < s; ++i)
    {
        out << nm.at(i) << "," << spectrum.at(i) << "\n";
    } 
    
}
void MainWindow::saveAsRelative()
{
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Save relative spectrum"),
        desktopPath + "/RelativeSpectrum1.csv",
        tr("Text files (*.txt, *.csv);; Any file(*)")
    );
    QFile savedFile(fileName);
    if (!savedFile.open(QIODevice::NewOnly | QIODevice::Text))
    {
        //return
    }
    auto nm{ m_spectrometer->getLastWavelengths() };
    auto spectrum{ m_spectrometer->getLastSpectrum() };
    //auto relativeSpectrum{ m_spectrumProcessor->toRelative(m_spectrometr->getLastSpectrum()) };
    auto relativeSpectrum{ m_spectrumProcessor->toRelative(spectrum) };
    std::size_t s{ nm.size() };
    QTextStream out(&savedFile);// out FROM programm
    out << "nm,intensity\n";
    for (std::size_t i{ 0 }; i < s; ++i)
    {
        out << nm.at(i) << "," << relativeSpectrum.at(i) << "\n";
    }
}
void MainWindow::openCalibration()
{
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open calibration file"),
        desktopPath,
        tr("calibration file (*.cal);; Any file(*)")
    );

    QFile openedFile(fileName);
    if (!openedFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //return
    }
    QTextStream in(&openedFile); // IN to our program

    if(!in.atEnd())
        QString line = in.readLine(); // first ROW (nm, Coefficient)
    

    std::vector<double> calibrationNm{};
    std::vector<double> calibrationCoeff{};
    while (!in.atEnd())
    {
        QString line{ in.readLine() };
        QStringList splited{ line.split(",") };
        double nm{ splited.at(0).toDouble() };
        double coeff{ splited.at(1).toDouble() };
        calibrationNm.push_back(nm);
        calibrationCoeff.push_back(coeff);

    }
    //auto calibration = std::pair(calibrationNm, calibrationCoeff);
    m_spectrumProcessor->setCalibrationWavelengts(calibrationNm);
    m_spectrumProcessor->setCalibrationCoeff(calibrationCoeff);
    

}