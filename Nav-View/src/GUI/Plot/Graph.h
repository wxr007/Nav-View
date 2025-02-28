#pragma once

#include <QWidget>
#include "ui_Graph.h"
#include "MonitorBar.h"

class Graph : public QWidget
{
	Q_OBJECT

public:
	Graph(QWidget *parent = nullptr);
	~Graph();
public slots:
	void onUpdate(int type);
private:
	Ui::GraphClass ui;
	//MonitorBar *angles_bar;
	//MonitorBar *ang_rate_bar;
	MonitorBar *accel_bar;
    MonitorBar *gyro_bar;
	MonitorBar *temp_bar;
};
