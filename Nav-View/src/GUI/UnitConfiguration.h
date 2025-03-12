#pragma once

#include <QWidget>
#include "ui_UnitConfiguration.h"

class UnitConfiguration : public QWidget
{
	Q_OBJECT

public:
	UnitConfiguration(QWidget *parent = nullptr);
	~UnitConfiguration();

public slots:
	void onUpdateValues(int type, QByteArray value);
    void on_GetAllValues_clicked();	
	void on_SetAllValues_clicked();
private:
	Ui::UnitConfigurationClass ui;
};
