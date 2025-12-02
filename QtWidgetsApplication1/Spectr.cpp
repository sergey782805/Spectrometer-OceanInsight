#include "spectr.h"
#include "GraphModel.h"
Spectr::Spectr(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    m_model = new GraphModel;

    //ui.quickWidget->setInitialProperties({ {"model", QVariant::fromValue(&model)} });


    //ui.quickWidget->setSource(QUrl::fromLocalFile("Graph.qml"));


    //QObject::connect(ui.pushButton, &QPushButton::clicked, this, &Spectr::on_update_button_cliced);
    //QObject::connect(&model, &GraphModel::maxXChanged, this, &Spectr::execFromModel); //TESTING
    //QObject::connect(ui.pushButton, &QPushButton::clicked, &model, &GraphModel::setMaxX);
    




    showModel();



    resize(800, 600);

    
}



int Spectr::showModel()
{
    if (!m_model->isReady())
    {
        ui.textBrowser->append("INIT failed!: " + QString::number(m_model->isReady()));
        return -1;
    }
    ui.textBrowser->append("-----INTIT STARTING-----");
    ui.textBrowser->append("Device count: " + QString::number(m_model->getdeviceCount()));
    if (m_model->getdeviceCount() < 0)
    {
        return -1;
    }  
    ui.textBrowser->append("Device Id Count: " + QString::number(m_model->getDeviceIdCount()));
    ui.textBrowser->append("Device name: " + m_model->getDeviceName());
    ui.textBrowser->append("Pixel count: " + QString::number(m_model->getPixelCount()));
    ui.textBrowser->append("-----INTIT END-----");

    return 0;
}

Spectr::~Spectr()
{
    delete m_model;
    m_model = nullptr;
}

