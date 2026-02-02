#include "spectr.h"


Spectr::Spectr(QWidget *parent)
    : QMainWindow(parent), m_model {nullptr}, m_spectrometr {nullptr}
{
    ui.setupUi(this);

    m_model = new GraphModel();
    m_spectrometr = new Spectrometr();
    m_spectrumProcessor = new SpectrumProcessor();

    ui.quickWidget->engine()->rootContext()->setContextProperty("myModel", m_model);
  
    ui.quickWidget->setSource(QUrl::fromLocalFile("Graph.qml"));

    QObject::connect(ui.pushButton, &QPushButton::clicked, this, &Spectr::updateGraph);
    QObject::connect(ui.averageValue, &QSpinBox::valueChanged, this, &Spectr::changeAverage);
    QObject::connect(ui.integrationTimeValue, &QSpinBox::valueChanged, this, &Spectr::changeIntegrationTime);

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

void Spectr::updateGraph()
{


    QList<QPointF> data;
    std::vector<double> wavelenghts = m_spectrometr->readWaveLengths();
    std::vector<double> spectrum = m_spectrometr->readSpectrum();

    //std::vector<double> spectrumRelative = m_spectrumProcessor->toRelative(spectrum);


    wavelenghts = { 400, 500, 600, 650, 700, 750, 800, 850, 900, 950, 1000 };
    spectrum = { 0.1, 0.3, 0.6, 0.1, 0.9, 0.8, 2.5, 0.2, 0.25, 0.5, 0.05 };

    std::size_t lo = m_spectrometr->getIndexOfWavelenght(350.0);
    std::size_t hi = m_spectrometr->getIndexOfWavelenght(800.0);

    lo = { 0 };
    hi = { 10 };

    double PPFD = m_spectrumProcessor->PPFD(wavelenghts, spectrum, lo, hi);

    std::vector<double> relativeSpectrum = m_spectrumProcessor->toRelative(spectrum);

    data = m_spectrumProcessor->toQList(wavelenghts, relativeSpectrum);
    //data = m_spectrometr->getNewSpectrum(); // uncomment to read from spectometr
    m_model->setData(data);
    ui.PPFD->setValue(PPFD);
    ui.textBrowser->append(QString::number(m_spectrumProcessor->PPFD(wavelenghts, spectrum, lo, hi)));
    
}

void Spectr::changeAverage()
{
    m_spectrometr->setAverageFactor(ui.averageValue->value());
    ui.textBrowser->append("Average values now is : " + QString::number(ui.averageValue->value()));
}
void Spectr::changeIntegrationTime()
{
    m_spectrometr->setIntegrationTime(ui.integrationTimeValue->value());
    ui.textBrowser->append("Integration time now is: " + QString::number(ui.integrationTimeValue->value()));
}

