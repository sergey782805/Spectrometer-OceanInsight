#pragma once

#include <QtWidgets/QMainWindow>
#include <QQuickWidget>
#include <QtGraphs/qsplineseries.h>
#include "GraphModel.h"
#include "Spectrometr.h"
#include "SpectrumProcessor.h"
#include <QtQml/qqmlengine.h>
#include <qqmlcontext.h>

#include <qqml.h>



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
    Spectrometr* m_spectrometr;
    SpectrumProcessor* m_spectrumProcessor;
    
    void updateGraph();
    void changeAverage();
    void changeIntegrationTime();
};

