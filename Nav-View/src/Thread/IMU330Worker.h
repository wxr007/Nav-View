#pragma once

#include <QObject>
#include "SerialThread.h"

class IMU330Worker  : public SerialWorker
{
	Q_OBJECT

public:
	IMU330Worker(QObject *parent = nullptr, DataParser* dataparser = nullptr);
	~IMU330Worker();
	void sendPacket(int8_t msgid);
public slots:
	void onReopen();
	void onJustClosePort();
	void onJustOpenPort();
signals:
	void sgnSendData(const QByteArray data);
	void sgnReopen();
	void sgnJustClosePort();
	void sgnJustOpenPort();
};
