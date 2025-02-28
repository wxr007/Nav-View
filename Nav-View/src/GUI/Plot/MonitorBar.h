#pragma once

#include <QWidget>
#include "ui_MonitorBar.h"
#include "widgetplot2d.h"

class MonitorBar : public QWidget
{
	Q_OBJECT

public:
	MonitorBar(QWidget *parent = nullptr,QString name = "MonitorBar");
	~MonitorBar();
	void initGraphName(QStringList name);
	void setYRange(double lower, double upper);
    void addData(QString name,int index, double value);
public slots:
	void onResetRange();
private:
	Ui::MonitorBarClass ui;
	widgetplot2d* m_plot;
	double m_lower;
    double m_upper;
	QVector<QLineEdit*> m_lineEditList;
};
