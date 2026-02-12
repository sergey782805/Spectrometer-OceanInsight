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
    QObject::connect(ui.averageValue, &QSpinBox::valueChanged, this, &Spectr::changeAverage);
    QObject::connect(ui.integrationTimeValue, &QSpinBox::valueChanged, this, &Spectr::changeIntegrationTime);
    QObject::connect(ui.readDark_button, &QPushButton::clicked, this, &Spectr::readDark);
    QObject::connect(ui.readCorrectedSpectrum_button, &QPushButton::clicked, this, &Spectr::readCorrectedSpectrum);
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
}

void Spectr::changeAverage()
{
    m_spectrometr->setAverageFactor(ui.averageValue->value());
    //ui.textBrowser->append("Average values now is : " + QString::number(ui.averageValue->value()));
}
void Spectr::changeIntegrationTime()
{
    m_spectrometr->setIntegrationTime(ui.integrationTimeValue->value());
    //ui.textBrowser->append("Integration time now is: " + QString::number(ui.integrationTimeValue->value()));
}

