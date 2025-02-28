#pragma once

#include <QObject>
#include "imu_330\IMU330_Decoder.h"

class DataParser  : public QObject
{
	Q_OBJECT

public:
	DataParser(QObject *parent);
	~DataParser();
	void init();
	void setDataFormat(int index);
	void parseData(QByteArray& array);
protected:
	void processIMU330(QByteArray array);
private:
	int m_DataFormat;
	IMU330_Decoder* m_IMU330_Decoder;
signals:
	void sgnUpdate(int type);
	void sgnUpdateValues(int type,QString value);
};
