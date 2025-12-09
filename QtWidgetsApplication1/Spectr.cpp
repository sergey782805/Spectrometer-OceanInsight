#include "spectr.h"


Spectr::Spectr(QWidget *parent)
    : QMainWindow(parent), m_model {nullptr}, m_spectrometr {nullptr}
{
    ui.setupUi(this);


    m_model = new GraphModel();
    m_spectrometr = new Spectrometr();

    ui.quickWidget->engine()->rootContext()->setContextProperty("myModel", m_model);
  
    ui.quickWidget->setSource(QUrl::fromLocalFile("Graph.qml"));

    QObject::connect(ui.pushButton, &QPushButton::clicked, this, &Spectr::updateGraph);
    QObject::connect(ui.averageValue, &QSpinBox::valueChanged, this, &Spectr::changeAverage);
    QObject::connect(ui.integrationTimeValue, &QSpinBox::valueChanged, this, &Spectr::changeIntegrationTime);

    resize(800, 600);
}
Spectr::~Spectr()
{
    delete m_spectrometr;
    m_spectrometr = nullptr;
    delete m_model;
    m_model = nullptr;
}

void Spectr::updateGraph()
{


    //m_spectrometr->setAverageFactor(10); // value from UI element!
    //m_spectrometr->setIngertationTime(800000); // value from UI element!

    QList<QPointF> data{QPointF(1, 0.1) , QPointF(2, 0.3), QPointF(3, 0.6)};
    
    m_model->setData(data);
    
}

void Spectr::changeAverage()
{
    m_spectrometr->setAverageFactor(ui.averageValue->value());
    ui.textBrowser->append("Average values now is : " + QString::number(ui.averageValue->value()));
}
void Spectr::changeIntegrationTime()
{
    m_spectrometr->setIngertationTime(ui.integrationTimeValue->value());
    ui.textBrowser->append("Integration time now is: " + QString::number(ui.integrationTimeValue->value()));
}

