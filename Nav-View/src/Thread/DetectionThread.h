#pragma once

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class DetectionThread  : public QThread
{
	Q_OBJECT

public:
	DetectionThread(QObject *parent);
	~DetectionThread();
	void run();
	void heartbeat();
public:
    QMutex m_Mutex;
    QWaitCondition m_WaitCondition;
private:
	bool m_isFound;
	QString m_portName;
signals:
	void sgnSendData(const QByteArray data);
	void sgnShowStatus(const QString status);
};
