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
    QObject::connect(ui.autoIntegrationTIme, &QPushButton::clicked, this, &MainWindow::integrationTimeAutoSelect);
    QObject::connect(ui.filter, &QComboBox::currentIndexChanged, this, &MainWindow::updateGraphOnFilterChange);
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
    m_spectrometer->readBiasDarkSpectrum();
    ui.textBrowser->append("<b style='color: green'>  BIAS Dark Spectrum read</b>");
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
    //wavelengths = {
    //179.0, 187.9, 196.8, 205.7, 214.5, 223.4, 232.3, 241.2, 250.1, 258.9,
    //267.8, 276.7, 285.6, 294.5, 303.3, 312.2, 321.1, 330.0, 338.9, 347.7,
    //356.6, 365.5, 374.4, 383.3, 392.1, 401.0, 409.9, 418.8, 427.7, 436.5,
    //445.4, 454.3, 463.2, 472.1, 481.0, 489.8, 498.7, 507.6, 516.5, 525.4,
    //534.2, 543.1, 552.0, 560.9, 569.8, 578.6, 587.5, 596.4, 605.3, 614.2,
    //623.0, 631.9, 640.8, 649.7, 658.6, 667.5, 676.3, 685.2, 694.1, 703.0,
    //711.9, 720.7, 729.6, 738.5, 747.4, 756.3, 765.1, 774.0, 782.9, 791.8,
    //800.7, 809.5, 818.4, 827.3, 836.2, 845.1, 853.9, 862.8, 871.7, 880.6,
    //889.5, 898.4, 907.2, 916.1, 925.0, 933.9, 942.8, 951.6, 960.5, 969.4,
    //978.3, 987.2, 996.0, 1004.9, 1013.8, 1022.7, 1031.6, 1040.4, 1049.3, 1058.0
    //};
    //correctedSpectrum = {
    //    49.6, 51.3, 50.2, 48.7, 52.1, 50.4, 47.9, 45.2, 43.8, 41.1,
    //    38.4, 35.6, 31.9, 29.4, 27.2, 23.9, 21.5, 20.8, 22.3, 24.1,
    //    27.6, 30.2, 33.4, 34.9, 36.1, 34.8, 32.1, 30.5, 27.9, 23.4,
    //    21.1, 18.4, 15.2, 13.9, 11.2, 12.4, 14.1, 15.6, 17.2, 16.1,
    //    14.8, 12.3, 10.5, 8.2, 6.4, 4.9, 3.1, 2.4, 4.8, 8.9,
    //    15.4, 25.1, 38.6, 52.4, 67.9, 78.2, 82.4, 80.1, 73.4, 62.9,
    //    51.2, 42.6, 35.1, 31.4, 29.8, 30.6, 34.2, 39.1, 45.4, 51.9,
    //    53.2, 51.7, 46.4, 39.2, 31.1, 24.6, 19.4, 15.2, 12.9, 11.4,
    //    10.8, 11.5, 12.2, 13.4, 14.9, 15.3, 16.2, 15.8, 14.1, 14.9,
    //    15.2, 16.4, 15.1, 14.3, 15.9, 14.8, 13.9, 15.2, 14.4, 15.1
    //};
    switch (ui.filter->currentIndex())
    {
    case 1:
        correctedSpectrum = m_spectrumProcessor->savitzkyGolayFilter(correctedSpectrum, 9);
        break;
    case 2: 
        correctedSpectrum = m_spectrumProcessor->savitzkyGolayFilter(correctedSpectrum, 11);
        break;
    case 3: 
        correctedSpectrum = m_spectrumProcessor->savitzkyGolayFilter(correctedSpectrum, 13);
        break;
    case 4: 
        correctedSpectrum = m_spectrumProcessor->savitzkyGolayFilter(correctedSpectrum, 15);
        break;
    case 5: 
        correctedSpectrum = m_spectrumProcessor->savitzkyGolayFilter(correctedSpectrum, 17);
        break;
    case 6: 
        correctedSpectrum = m_spectrumProcessor->savitzkyGolayFilter(correctedSpectrum, 19);
        break;
    case 7: 
        correctedSpectrum = m_spectrumProcessor->savitzkyGolayFilter(correctedSpectrum, 21);
        break;
    case 8: 
        correctedSpectrum = m_spectrumProcessor->savitzkyGolayFilter(correctedSpectrum, 23);
        break;
    case 9: 
        correctedSpectrum = m_spectrumProcessor->savitzkyGolayFilter(correctedSpectrum, 25);
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
    ui.PFD->setValue(pfd);
    double ppfd{ m_spectrumProcessor->PARsum(calibratedSpectrum, 400.069 ,699.978) };
    ui.PPFD->setValue(ppfd);

    ui.textBrowser->append("<b style='color: green'> Corrected Spectrum read</b>");
    ui.textBrowser->append("<b style='color: red'> integration time should be set to: </b>" + QString::number(m_spectrometer->detectIntegrationTime()));
    //ui.textBrowser->append("<b style='color: orange'> PFD: </b>" + QString::number(pfd));
    //ui.textBrowser->append("<b style='color: orange'> PPFD: </b>" + QString::number(ppfd));
}
void MainWindow::integrationTimeAutoSelect()
{
    if(!m_spectrometer->isReady())
        return

    ui.integrationTimeValue->setReadOnly(true);
    ui.averageValue->setReadOnly(true);
    ui.readDark_button->setEnabled(false);
    ui.readCorrectedSpectrum_button->setEnabled(false);

    const unsigned int neededAverage{ static_cast<unsigned int> (ui.averageValue->value()) };
    m_spectrometer->setAverageFactor(1);
    unsigned long startIntegrationTime{ static_cast<unsigned long> (ui.integrationTimeValue->value()) };
    //There should be check for non empty dark spectrum

    if (!m_spectrometer->getDarkSpectrum().empty())
    {

        QThread* thread = QThread::create([this, neededAverage, startIntegrationTime]() {
            unsigned long currIntegrationTime = startIntegrationTime;
            unsigned long newIntegrationTime{ 0 };
            do
            {


                m_spectrometer->readCorrectedSpectrum();
                newIntegrationTime = m_spectrometer->detectIntegrationTime();
                QMetaObject::invokeMethod(this, [this, newIntegrationTime]() {

                    ui.integrationTimeValue->setValue(newIntegrationTime);
                    changeIntegrationTime(); 
                    }, Qt::QueuedConnection);

                if (newIntegrationTime == currIntegrationTime)
                {
                    break;
                }
                currIntegrationTime = newIntegrationTime;
                QThread::msleep(20);
                //ui.integrationTimeValue->setValue(newIntegrationTime);
                //changeIntegrationTime();
                //m_spectrometer->setIntegrationTime(newIntegrationTime); // change to MainWindow changeIntegrationTime() function!
                //updating UI to see difference 


            } while (true);
            //doing the same but for current average factor
            m_spectrometer->setAverageFactor(neededAverage);
            do
            {

                m_spectrometer->readCorrectedSpectrum();
                newIntegrationTime = m_spectrometer->detectIntegrationTime();
                QMetaObject::invokeMethod(this, [this, newIntegrationTime]() {
                    ui.integrationTimeValue->setValue(newIntegrationTime);
                    changeIntegrationTime(); 
                    }, Qt::QueuedConnection);

                if (newIntegrationTime == currIntegrationTime)
                {
                    break;
                }
                currIntegrationTime = newIntegrationTime;
                QThread::msleep(20);
                //ui.integrationTimeValue->setValue(newIntegrationTime);
                //changeIntegrationTime();
                //m_spectrometer->setIntegrationTime(newIntegrationTime);
                //updating UI to see difference


            } while (true);

            });


        QObject::connect(thread, &QThread::finished, this, [this]()
            {
                ui.readCorrectedSpectrum_button->setEnabled(true);
                ui.readDark_button->setEnabled(true);
                ui.averageValue->setReadOnly(false);
                ui.integrationTimeValue->setReadOnly(false);
            });
        QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);

        thread->start();
    }
}
void MainWindow::updateGraphOnFilterChange()
{
    std::vector<double> wavelengths{ m_spectrometer->getLastWavelengths() };
    std::vector<double> lastSpectrum{ m_spectrometer->getLastSpectrum() };

    switch (ui.filter->currentIndex())
    {
    case 1: // savitzkyGolayFilter9
        lastSpectrum = m_spectrumProcessor->savitzkyGolayFilter(lastSpectrum, 9);
        break;
    case 2: // savitzkyGolayFilter9
        lastSpectrum = m_spectrumProcessor->savitzkyGolayFilter(lastSpectrum, 11);
        break;
    case 3: // savitzkyGolayFilter9
        lastSpectrum = m_spectrumProcessor->savitzkyGolayFilter(lastSpectrum, 13);
        break;
    case 4: // savitzkyGolayFilter9
        lastSpectrum = m_spectrumProcessor->savitzkyGolayFilter(lastSpectrum, 15);
        break;
    case 5: // savitzkyGolayFilter9
        lastSpectrum = m_spectrumProcessor->savitzkyGolayFilter(lastSpectrum, 17);
        break;
    case 6: // savitzkyGolayFilter9
        lastSpectrum = m_spectrumProcessor->savitzkyGolayFilter(lastSpectrum, 19);
        break;
    case 7: // savitzkyGolayFilter9
        lastSpectrum = m_spectrumProcessor->savitzkyGolayFilter(lastSpectrum, 21);
        break;
    case 8: // savitzkyGolayFilter9
        lastSpectrum = m_spectrumProcessor->savitzkyGolayFilter(lastSpectrum, 23);
        break;
    case 9: // savitzkyGolayFilter9
        lastSpectrum = m_spectrumProcessor->savitzkyGolayFilter(lastSpectrum, 25);
        break;
    default: // 0, no filter
        break;
    }
    std::vector<double> relativeSpectrum{ m_spectrumProcessor->toRelative(lastSpectrum) };

    QList<QPointF> data = m_spectrumProcessor->toQList(wavelengths, relativeSpectrum);
    m_model->setData(data);

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

	const unsigned long minIntegrationTime{ m_spectrometer->getMinIntegrationTime()};
	const unsigned long newIntegrationTime{ static_cast<unsigned long> (ui.integrationTimeValue->value())};
    
    const std::vector<double> biasDarkSpectrum{ m_spectrometer->getBiasDarkSpectrum() };
    const std::vector<double> pureDarkSpectrum{ m_spectrometer->getPureDarkSpectrum() };

    if (!biasDarkSpectrum.empty() && !pureDarkSpectrum.empty())
    {
       
        const std::vector<double> adjustedDarkSpectrum{ m_spectrumProcessor->adjustDarkSpectrum(biasDarkSpectrum, pureDarkSpectrum, minIntegrationTime, newIntegrationTime) };

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
        return;
    }
    QTextStream in(&openedFile); // IN to our program
    unsigned long calibrationIntegrationTime{ 0 }; 
    std::vector<double> calibrationNm{};
    std::vector<double> calibrationCoeff{};

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if (line.isEmpty())
            continue;

       
        if (line.startsWith("#"))
        {
            if (line.contains("IntegrationTime_us:"))
            {
                int colonIdx = line.indexOf(":");
                if (colonIdx != -1)
                {
                    QString valueStr = line.mid(colonIdx + 1).trimmed();
                    bool ok = false;
                    calibrationIntegrationTime = valueStr.toULong(&ok);
                    if (!ok)
                    {
                        calibrationIntegrationTime = 0;
                    }
                }
            }
            continue; 
        }

        
        if (line.contains("nm") || line.contains("Coefficient"))
            continue;

        
        QStringList splited = line.split(",");
        if (splited.size() >= 2) 
        {
            bool okNm = false, okCoeff = false;
            double nm = splited.at(0).toDouble(&okNm);
            double coeff = splited.at(1).toDouble(&okCoeff);

            if (okNm && okCoeff)
            {
                calibrationNm.push_back(nm);
                calibrationCoeff.push_back(coeff);
            }
        }
    }

   
    m_spectrumProcessor->setCalibrationWavelengts(calibrationNm);
    m_spectrumProcessor->setCalibrationCoeff(calibrationCoeff);
    //m_spectrumProcessor->setCalibrationIntegrationTime(calibrationIntegrationTime);
    

}