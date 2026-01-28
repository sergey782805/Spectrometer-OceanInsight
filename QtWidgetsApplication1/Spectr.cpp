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

    //maybe update average and int
    //m_spectrometr->setAverageFactor(ui.averageValue->value()); // value from UI element!
    //m_spectrometr->setIntegrationTime(ui.integrationTimeValue->value()); // value from UI element!

    QList<QPointF> data{ QPointF(200.0, 0.1) , QPointF(300.0, 0.3), QPointF(400.0, 0.6), QPointF(500, 0.4), QPointF(600, 0.9) };
    data.clear();
    //data.clear();
    std::vector<double> wavelenghts = m_spectrometr->readWaveLengths();
    std::vector<double> spectrum = m_spectrometr->readSpectrum();

    wavelenghts = { 200.0, 300.0, 400.0, 500.0, 600.0 };
    spectrum = { 0.1, 0.3, 0.6, 0.4, 0.9 };

    std::size_t lo = m_spectrometr->getIndexOfWavelenght(350);
    std::size_t hi = m_spectrometr->getIndexOfWavelenght(800);

    double PPFD = m_spectrumProcessor->PPFD(wavelenghts, spectrum, lo, hi);

    //std::vector<double> relativeSpectrum = m_spectrumProcessor->toRelative(spectrum);

    data = m_spectrumProcessor->toQList(wavelenghts, spectrum);
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

