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
/// 线程中获取的数据都放到DataCache中，
/// 再通知窗体从DataCache中取数据。
/// </summary>

class ThreadManager : public QObject
{
    Q_OBJECT
public:
    static ThreadManager& Instance();
    ~ThreadManager();
private:
    ThreadManager();   // 构造函数私有化
    ThreadManager(ThreadManager const&); // 禁止拷贝构造
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
