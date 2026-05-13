#pragma once

#include <QtWidgets/QMainWindow>
#include <QQuickWidget>
#include <QtGraphs/qsplineseries.h>
#include "GraphModel.h"
#include "Spectrometer.h"
#include "SpectrumProcessor.h"
#include <QtQml/qqmlengine.h>
#include <qqmlcontext.h>
#include <qfiledialog.h>
#include <qstandardpaths.h>
#include <qqml.h>

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    
private:
    Ui::MainWindow ui;
    
    GraphModel* m_model;
    Spectrometer* m_spectrometer;
    SpectrumProcessor* m_spectrumProcessor;
    
    void readDark();
    void readCorrectedSpectrum();
    void changeAverage();
    void changeIntegrationTime();
    void saveAs();
    void saveAsRelative();
    void openCalibration();
};

