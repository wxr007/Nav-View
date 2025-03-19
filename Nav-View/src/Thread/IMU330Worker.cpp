#include "IMU330Worker.h"

#define SYNC1 0xA5
#define SYNC2 0x96

static uint16_t crc16_ccitt_false(uint8_t* buf, uint16_t length)
{
    uint16_t crc = 0xFFFF; // seed is 0xFFFF
    for (int i = 0; i < length; i++) {
        crc ^= buf[i] << 8;
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021; // poly is 0x1021
            }
            else {
                crc = crc << 1;
            }
        }
    }
    return crc;
}

IMU330Worker::IMU330Worker(QObject *parent, DataParser* dataparser)
	: SerialWorker(parent, dataparser)
{
    connect(this, SIGNAL(sgnSendData(const QByteArray)), this, SLOT(onWrite(const QByteArray)), Qt::QueuedConnection);
    connect(this, SIGNAL(sgnReopen()), this, SLOT(onReopen()), Qt::QueuedConnection);
    connect(this, SIGNAL(sgnJustClosePort()), this, SLOT(onJustClosePort()), Qt::QueuedConnection);
    connect(this, SIGNAL(sgnJustOpenPort()), this, SLOT(onJustOpenPort()), Qt::QueuedConnection);
}

IMU330Worker::~IMU330Worker()
{}
//串口协议格式
//| sync1 | sync2 | msgid | length | data | crc16(lsb first) |
//|  1B   | 1B    | 1B    | 1B     | nB   | 2B     
void IMU330Worker::sendPacket(int8_t msgid)
{
    QByteArray msg;
    msg.append(SYNC1);
    msg.append(SYNC2);
    msg.append((char)msgid);
    msg.append((char)0);

    uint16_t crc = crc16_ccitt_false((uint8_t*)msg.data() + 2, 2);
    char crcb[2] = { 0 };
    memcpy(crcb, &crc, 2);
    msg.append(crcb, 2);
    emit sgnSendData(msg);
}

void IMU330Worker::onJustClosePort()
{
    m_QextSerialPort->close();
}

void IMU330Worker::onJustOpenPort()
{
    if(m_QextSerialPort->isOpen() == false)
        m_QextSerialPort->open(QIODevice::ReadWrite);
}

void IMU330Worker::onReopen()
{
    m_QextSerialPort->close();
    m_QextSerialPort->open(QIODevice::ReadWrite);
}
