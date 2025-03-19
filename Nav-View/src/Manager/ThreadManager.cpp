#include "ThreadManager.h"

std::once_flag ThreadManager::once_flag;
std::unique_ptr<ThreadManager> ThreadManager::instance;

ThreadManager& ThreadManager::Instance() {
    std::call_once(once_flag, [&]() {
        instance.reset(new ThreadManager());
    });
    return *instance;
}
ThreadManager::ThreadManager()
    : QObject(NULL)
{
    m_DetectionThread = new DetectionThread(NULL);
    m_DataParser = new DataParser(NULL);
    m_IMU330Worker = new IMU330Worker(NULL, m_DataParser);
    m_SerialThread = new SerialThread(NULL, m_IMU330Worker);
}
ThreadManager::~ThreadManager()
{
    m_SerialThread->stop();
}