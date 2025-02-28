#include "MonitorBar.h"
#include "ThreadManager.h"
#include "DataCache.h"
#include <QFontDataBase>

MonitorBar::MonitorBar(QWidget *parent, QString name)
	: QWidget(parent)
{
	ui.setupUi(this);
	//引入图形字体
	int fontId = QFontDatabase::addApplicationFont(":/res/fontawesome-webfont.ttf");
	QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
	QFont iconFont = QFont(fontName);
	ui.reset_btn->setFont(iconFont);
	ui.reset_btn->setText(QChar(0xf021));
	ui.reset_btn->setToolTip(u8"Reset Y range");

	m_lower = 0.0;
    m_upper = 0.0;

	m_plot = new widgetplot2d(this);
	m_plot->setYRange(-10, 10);
	m_plot->setXTimeTicker();
	m_plot->setYLabel(name);
	ui.verticalLayout_plot->addWidget(m_plot);

	connect(ui.reset_btn, SIGNAL(clicked()), this, SLOT(onResetRange()));
	//connect(ThreadManager::Instance().m_DataParser, SIGNAL(sgnUpdate(int)), this, SLOT(onUpdate(int)), Qt::QueuedConnection);
}

MonitorBar::~MonitorBar()
{}

void MonitorBar::initGraphName(QStringList name)
{
	m_plot->initGraphName(name);
	ui.gridLayout->setColumnStretch(0, 1);
	m_lineEditList.clear();
	for (int i = 0; i < name.size(); i++) {
        QLabel *label = new QLabel(name[i], this);
        ui.gridLayout->addWidget(label, i + 1, 0, 1, 1);
		QLineEdit *lineEdit = new QLineEdit(this);
		m_lineEditList.push_back(lineEdit);
        ui.gridLayout->addWidget(lineEdit, i + 1, 1, 1, 1);
	}
}

void MonitorBar::setYRange(double lower, double upper)
{
	m_lower = lower;
    m_upper = upper;
	m_plot->setYRange(m_lower, m_upper);
}

void MonitorBar::addData(QString name, int index, double value)
{
	m_plot->addDataByTime(name, value);
	if(index < m_lineEditList.size())
		m_lineEditList[index]->setText(QString::number(value));
}

void MonitorBar::onResetRange()
{
	m_plot->setYRange(m_lower, m_upper);
	m_plot->replot();
}