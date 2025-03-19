#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_NavView.h"
#include "CustomMdiSubWindow.h"
#include "SerialportWidget.h"
#include "LogFilePath.h"
#include "UnitConfiguration.h"
#include "FieldSettings.h"
#include "Plot/Graph.h"

class NavView : public QMainWindow
{
    Q_OBJECT

public:
    NavView(QWidget *parent = nullptr);
    ~NavView();
    void initWinPos();
public slots:
    void on_actionSetup_triggered();
    void on_actionLogFile_triggered();
    void on_actionConfiguration_triggered();
    void on_actionGraph_triggered();
    void on_actionFieldSettings_triggered();
    void on_actionRecord_triggered();
    void on_statusBar_messageChanged(const QString message);
private:
    Ui::NavViewClass ui;
    SerialportWidget *serialportWidget;
    CustomMdiSubWindow* serialportsSubWindow;
    LogFilePath* logFilePath;
    CustomMdiSubWindow* logFileSubWindow;
    UnitConfiguration* unitConfiguration;
    CustomMdiSubWindow* unitConfigSubWindow;
    Graph* graph;
    CustomMdiSubWindow* graphSubWindow;
    FieldSettings* fieldSettings;
    CustomMdiSubWindow* fieldSettingsSubWindow;
};
