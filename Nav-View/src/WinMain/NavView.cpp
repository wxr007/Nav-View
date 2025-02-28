#include "NavView.h"
#include <QMdiSubWindow>
#include <LogManager.h>

NavView::NavView(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    this->setCentralWidget(ui.mdiArea);
    ui.mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui.mdiArea->setViewMode(QMdiArea::SubWindowView);

    serialportWidget = new SerialportWidget();
    serialportsSubWindow = new CustomMdiSubWindow();
    serialportsSubWindow->setWidget(serialportWidget);
    ui.mdiArea->addSubWindow(serialportsSubWindow);
    serialportsSubWindow->hide();

    logFilePath = new LogFilePath();
    logFileSubWindow = new CustomMdiSubWindow();
    logFileSubWindow->setWidget(logFilePath);
    ui.mdiArea->addSubWindow(logFileSubWindow);
    logFileSubWindow->hide();

    unitConfiguration = new UnitConfiguration();
    unitConfigSubWindow = new CustomMdiSubWindow();
    unitConfigSubWindow->setWidget(unitConfiguration);
    ui.mdiArea->addSubWindow(unitConfigSubWindow);
    unitConfigSubWindow->hide();

    graph = new Graph();
    graphSubWindow = new CustomMdiSubWindow();
    graphSubWindow->setWidget(graph);
    ui.mdiArea->addSubWindow(graphSubWindow);
    graphSubWindow->hide();

    fieldSettings = new FieldSettings();
    fieldSettingsSubWindow = new CustomMdiSubWindow();
    fieldSettingsSubWindow->setWidget(fieldSettings);
    ui.mdiArea->addSubWindow(fieldSettingsSubWindow);
    fieldSettingsSubWindow->hide();

    initWinPos();
    //×î´ó»¯
    showMaximized();
}

NavView::~NavView()
{}

void NavView::initWinPos()
{
    QRect rect = QGuiApplication::screens()[0]->availableGeometry();
    int w = 1280;// rect.width() * 45 / 100;
    int h = 720;//rect.height() * 60/100;
    qDebug("window.w = %d,window.h=%d", w, h);
    rect.setRect((rect.width() - w) / 2, (rect.height() - h) / 2, w + 64, h);
    setGeometry(rect);
}

void NavView::on_actionSetup_triggered()
{
    serialportsSubWindow->show();
}
void NavView::on_actionLogFile_triggered()
{
    logFileSubWindow->show();
}

void NavView::on_actionConfiguration_triggered()
{
    unitConfigSubWindow->show();
}

void NavView::on_actionGraph_triggered()
{
    graphSubWindow->show();
}

void NavView::on_actionFieldSettings_triggered()
{
    fieldSettingsSubWindow->show();
}

void NavView::on_actionRecord_triggered()
{
    if (LogManager::getInstance()->getLogPath().isEmpty()) {
        logFileSubWindow->show();
    }
}

