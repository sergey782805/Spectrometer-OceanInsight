#include "spectr.h"


Spectr::Spectr(QWidget *parent)
    : QMainWindow(parent), m_model {nullptr}, m_spectrometr {nullptr}, m_spectrumProcessor{nullptr}
{
    ui.setupUi(this);

    m_model = new GraphModel();
    m_spectrometr = new Spectrometr();
    m_spectrumProcessor = new SpectrumProcessor();

    ui.quickWidget->engine()->rootContext()->setContextProperty("myModel", m_model);
  
    ui.quickWidget->setSource(QUrl::fromLocalFile("Graph.qml"));

    //QObject::connect(ui.pushButton, &QPushButton::clicked, this, &Spectr::updateGraph);
    QObject::connect(ui.averageValue, &QSpinBox::editingFinished, this, &Spectr::changeAverage);
    QObject::connect(ui.integrationTimeValue, &QSpinBox::editingFinished, this, &Spectr::changeIntegrationTime);
    QObject::connect(ui.readDark_button, &QPushButton::clicked, this, &Spectr::readDark);
    QObject::connect(ui.readCorrectedSpectrum_button, &QPushButton::clicked, this, &Spectr::readCorrectedSpectrum);
    QObject::connect(ui.actionSave_As, &QAction::triggered, this, &Spectr::saveAs);
    QObject::connect(ui.actionSave_As_Relative, &QAction::triggered, this, &Spectr::saveAsRelative);
    resize(800, 600);
}
Spectr::~Spectr()
{
    delete m_spectrumProcessor;
    m_spectrumProcessor = nullptr;
    delete m_spectrometr;
    m_spectrometr = nullptr;
    delete m_model;
    m_model = nullptr;
}
//This function is DEPRECATED, should be deleted 
void Spectr::updateGraph()
{

    QList<QPointF> data;
    std::vector<double> wavelenghts = m_spectrometr->readWaveLengths();
    std::vector<double> spectrum = m_spectrometr->readSpectrum();

    //std::vector<double> spectrumRelative = m_spectrumProcessor->toRelative(spectrum);


    //wavelenghts = { 400, 500, 600, 650, 700, 750, 800, 850, 900, 950, 1000 };
    //spectrum = { 0.1, 0.3, 0.6, 0.1, 0.9, 0.8, 2.5, 0.2, 0.25, 0.5, 0.05 };


    //double PPFD = m_spectrumProcessor->PPFD(wavelenghts, spectrum, lo, hi);

    std::vector<double> relativeSpectrum = m_spectrumProcessor->toRelative(spectrum);
    //relativeSpectrum = spectrum;
    data = m_spectrumProcessor->toQList(wavelenghts, relativeSpectrum);
    //data = m_spectrometr->getNewSpectrum(); // uncomment to read from spectometr
    m_model->setData(data);
    //ui.PPFD->setValue(PPFD);
    //ui.textBrowser->append(QString::number(m_spectrumProcessor->PPFD(wavelenghts, spectrum, lo, hi)));
    ui.textBrowser->append("<b style='color: orange'> Non-corrected Spectrum read </b>");
} // Deprecated
void Spectr::readDark()
{
    std::vector<double> wavelengths = m_spectrometr->readWaveLengths();
    std::vector<double> darkSpectrum = m_spectrometr->readDarkSpectrum();

    std::vector<double> darkRelative = m_spectrumProcessor->toRelative(darkSpectrum);
    //darkSpectrum = darkRelative;

    QList<QPointF> data = m_spectrumProcessor->toQList(wavelengths, darkRelative);
    m_model->setData(data);
    
    ui.textBrowser->append("<b style='color: green'> Dark Spectrum read</b>");

}
void Spectr::readCorrectedSpectrum()
{
    std::vector<double> wavelengths = m_spectrometr->readWaveLengths();
    std::vector<double> correctedSpectrum = m_spectrometr->readCorrectedSpectrum();

    std::vector<double> relativeCorrectedSpectrum = m_spectrumProcessor->toRelative(correctedSpectrum);
    //correctedSpectrum = relative;

    QList<QPointF> data = m_spectrumProcessor->toQList(wavelengths, relativeCorrectedSpectrum);
    m_model->setData(data);
    ui.textBrowser->append("<b style='color: green'> Corrected Spectrum read</b>");
    ui.textBrowser->append("<b style='color: red'> integration time should be set to: </b>" + QString::number(m_spectrometr->detectIntegrationTime()));
}

void Spectr::changeAverage()
{
    m_spectrometr->setAverageFactor(ui.averageValue->value());
    //ui.textBrowser->append("Min integration time: " + QString::number(m_spectrometr->getMinIntegrationTime()));
    //ui.textBrowser->append("Max integration time: " + QString::number(m_spectrometr->getMaxIntegrationTime()));
    //ui.textBrowser->append("Current integration time: " + QString::number(m_spectrometr->getIntegrationTime()));
    //ui.textBrowser->append("Average values now is : " + QString::number(ui.averageValue->value()));
}
void Spectr::changeIntegrationTime()
{
    m_spectrometr->setIntegrationTime(ui.integrationTimeValue->value());
    //ui.textBrowser->append("Integration time now is: " + QString::number(ui.integrationTimeValue->value()));
}

void Spectr::saveAs()
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

    auto nm{ m_spectrometr->getLastWavelengths()};
    auto spectrum{ m_spectrometr->getLastSpectrum()};
    
    std::size_t s{ nm.size() };

    QTextStream out(&savedFile);// out FROM programm
    out << "nm,counts\n";
    for (std::size_t i{ 0 }; i < s; ++i)
    {
        out << nm.at(i) << "," << spectrum.at(i) << "\n";
    } 
    
}
void Spectr::saveAsRelative()
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
    auto nm{ m_spectrometr->getLastWavelengths() };
    auto spectrum{ m_spectrometr->getLastSpectrum() };
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
