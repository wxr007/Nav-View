#include "DetectionThread.h"
#include "ThreadManager.h"
#include "DataCache.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo> 

DetectionThread::DetectionThread(QObject *parent)
	: QThread(parent)
	, m_isFound(false)
{
	m_portName.clear();
}

DetectionThread::~DetectionThread()
{}

void DetectionThread::run()
{
	QVector<int> baudRates = {460800, 115200, 230400, 78600, 57600, 38400, 19200 };//{ 19200, 38400, 57600, 78600, 115200, 230400, 460800 };
	int baudRate = 0;
	QString portName = "";
	while (true) {
		bool _auto = DataCache::Instance().m_auto_connect;
		if (_auto) {
			if (!ThreadManager::Instance().m_SerialThread->isRunning()) {
				if (QSerialPortInfo::availablePorts().size() > 0) {
					portName = QSerialPortInfo::availablePorts().at(0).portName();
					baudRate = baudRates[0];
					ThreadManager::Instance().m_SerialThread->setSerialPortParams(portName, baudRate);
					ThreadManager::Instance().m_SerialThread->start();
				}			
			}
			else {
				if (!m_isFound) {
					//通过QSerialPortInfo查找可用串口
					foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
					{
						portName = info.portName();
						if (m_portName.isEmpty()) {
							emit ThreadManager::Instance().m_IMU330Worker->sgnJustClosePort();
							foreach(baudRate, baudRates) {
								emit sgnShowStatus(QString("trying port %1 baudrate %2 ...").arg(portName).arg(baudRate));
								ThreadManager::Instance().m_SerialThread->setSerialPortParams(portName, baudRate);
								emit ThreadManager::Instance().m_IMU330Worker->sgnJustOpenPort();
								heartbeat();
								if (m_isFound) {
									break;
								}
							}
						}
						else {
							if (portName == m_portName) {
								emit ThreadManager::Instance().m_IMU330Worker->sgnJustClosePort();
								foreach(baudRate, baudRates) {
									emit sgnShowStatus(QString("trying port %1 baudrate %2 ...").arg(portName).arg(baudRate));
									ThreadManager::Instance().m_SerialThread->setSerialPortParams(portName, baudRate);
									emit ThreadManager::Instance().m_IMU330Worker->sgnJustOpenPort();
									heartbeat();
									if (m_isFound) {
										break;
									}
								}
							}
						}
						if (m_isFound) {
							m_portName = portName;
							break;
						}
					}
					if (!m_isFound) {
						m_portName = "";
					}
				}
				else {
					emit sgnShowStatus(QString("port %1 baudrate %2 connected").arg(portName).arg(baudRate));
					heartbeat();
				}
			}
		}
		QThread::sleep(1);
	}
}

void DetectionThread::heartbeat()
{
	m_Mutex.lock();
	DataCache::Instance().m_ret0x01 = false;
	//发送
	ThreadManager::Instance().m_IMU330Worker->sendPacket(0x01);
	//等待接收
	if (!DataCache::Instance().m_ret0x01) {
		m_WaitCondition.wait(&m_Mutex, 1000);
	}
	//接收
	m_Mutex.unlock();
	if (DataCache::Instance().m_ret0x01) {
		m_isFound = true;
	}
	else {
		m_isFound = false;
	}
}
