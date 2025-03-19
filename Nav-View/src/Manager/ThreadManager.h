#pragma once

#include <mutex>
#include "DetectionThread.h"
#include "SerialThread.h"
#include "DataParser.h"
#include "IMU330Worker.h"
#include <QObject>
#include <QMutex>
#include <QWaitCondition>

/// <summary>
/// �߳��л�ȡ�����ݶ��ŵ�DataCache�У�
/// ��֪ͨ�����DataCache��ȡ���ݡ�
/// </summary>

class ThreadManager : public QObject
{
    Q_OBJECT
public:
    static ThreadManager& Instance();
    ~ThreadManager();
private:
    ThreadManager();   // ���캯��˽�л�
    ThreadManager(ThreadManager const&); // ��ֹ��������
    ThreadManager& operator=(ThreadManager const&);
   
    static std::once_flag once_flag;
    static std::unique_ptr<ThreadManager> instance;
public:
    DataParser* m_DataParser;
    IMU330Worker* m_IMU330Worker;
    SerialThread* m_SerialThread;
    DetectionThread* m_DetectionThread;

    QMutex m_ReadFileMutex;
    QWaitCondition m_ReadFileCondition;
    bool m_ReadFileFlag;
};
