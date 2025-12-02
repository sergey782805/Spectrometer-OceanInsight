#pragma once

#include <QtWidgets/QMainWindow>
//#include <QQmlEngine>
#include <QQuickWidget>
#include <QtGraphs/qsplineseries.h>

#include "GraphModel.h"
#include <qqml.h>
#include <vector>
#include <memory>


#include "ui_Spectr.h"

class Spectr : public QMainWindow
{
    Q_OBJECT

public:
    Spectr(QWidget *parent = nullptr);
    ~Spectr();

    
private:
    Ui::SpectrClass ui;
    
    GraphModel* m_model;
    //GraphModel which will be passed to qml?

    int showModel();

};

