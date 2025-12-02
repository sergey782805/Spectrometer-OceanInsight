#include "spectr.h"
#include "GraphModel.h"
Spectr::Spectr(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //ui.quickWidget->setInitialProperties({ {"model", QVariant::fromValue(&model)} });

    ui.quickWidget->setSource(QUrl::fromLocalFile("Graph.qml"));


    QObject::connect(ui.pushButton, &QPushButton::clicked, this, &Spectr::on_update_button_cliced);
    //QObject::connect(&model, &GraphModel::maxXChanged, this, &Spectr::execFromModel); //TESTING
    //QObject::connect(ui.pushButton, &QPushButton::clicked, &model, &GraphModel::setMaxX);
    


    ui.textBrowser->append("Device Count: ");

    for (int i{ 0 }; i < 15; ++i)
    {
        ui.textBrowser->append("TEST " + QString::number(i));
    }


    showModel();



    resize(800, 600);

    
}

void Spectr::on_update_button_cliced()
{
    ui.textBrowser->append("Update button clicked!: ");
    model.setMaxX(1.0);
    ui.textBrowser->append(QString::number(model.getMaxX()));
}

void Spectr::execFromModel()
{
    ui.textBrowser->append("Executed from modelGraph!");
}


int Spectr::showModel()
{

    if (!model.isReady())
    {
        ui.textBrowser->append("INIT failed!: " + QString::number(model.isReady()));
        return -1;
    }

    ui.textBrowser->append("-----INTIT STARTING-----");
    ui.textBrowser->append("Device count: " + QString::number(model.getdeviceCount()));
    if (model.getdeviceCount() < 0)
    {
        return -1;
    }
        
    ui.textBrowser->append("Device Id Count: " + QString::number(model.getDeviceIdCount()));
    ui.textBrowser->append("Device name: " + model.getDeviceName());
    ui.textBrowser->append("Pixel count: " + QString::number(model.getPixelCount()));

    ui.textBrowser->append("-----INTIT END-----");

    return 0;
}

Spectr::~Spectr()
{}

