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
    m_DataParser = new DataParser(NULL);
    m_SerialThread = new SerialThread(NULL, m_DataParser);
}
ThreadManager::~ThreadManager()
{
    m_SerialThread->stop();
}