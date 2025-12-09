#include "Spectr.h"
#include "GraphModel.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QApplication app(argc, argv);

    //qmlRegisterType<GraphModel>("CustomTypes", 1, 0, "GraphModel");

    Spectr window;

    //QQuickWidget::setInitialProperties()

    window.show();
    return app.exec();
}
