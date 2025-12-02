#include "Spectr.h"
#include "GraphModel.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    


    qmlRegisterType<GraphModel>("CustomTypes", 1, 0, "GraphModel");

    Spectr window;


    //GraphModel model;
    
    //qmlRegisterType<GraphModel>("CustomTypes", 1, 0, "model");

    //QQuickWidget::setInitialProperties()

   

    window.show();
    return app.exec();
}
