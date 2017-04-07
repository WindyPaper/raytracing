#include "ThreadPool.h"

#if _WIN32
#include <windows.h>
#endif

ThreadPool::ThreadPool()
{
	initThreads();
}

ThreadPool::~ThreadPool()
{
	for (auto i = _thread_map.begin(); i != _thread_map.end(); ++i)
	{
		ThreadInfo *pThread = i->first;
		pThread->stopThread();
		delete (i->first);
	}
	_thread_map.clear();

	for (auto i = _tasks.begin(); i != _tasks.end(); ++i)
	{
		delete (*i);
	}
	_tasks.clear();
}

int ThreadPool::initThreads()
{
	int threadCount = getMachineProcessorNum();

	for (int i = 0; i < threadCount; ++i)
	{
		ThreadInfo *pThread = new ThreadInfo(this);
		pThread->startThread();

		_thread_map.insert(std::make_pair(pThread, i));
	}

	return threadCount;
}

void ThreadPool::addTask(TaskInfo *info)
{
	std::lock_guard<std::mutex> local_lock(_thread_mutex);
	_tasks.push_back(info);
	_condition_event.notify_one();
}

TaskInfo * ThreadPool::aquireTask()
{
	std::lock_guard<std::mutex> local_lock(_thread_mutex);
	if (_tasks.empty())
	{
		return NULL;
	}

	TaskInfo *ret = _tasks.back();
	_tasks.pop_back();
	return ret;
}

int ThreadPool::getMachineProcessorNum() const
{
#if _WIN32
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	if (info.dwNumberOfProcessors > 0)
		return info.dwNumberOfProcessors;
#endif

	return 4;
}

std::condition_variable & ThreadPool::getConditionEvent()
{
	return _condition_event;
}

std::mutex & ThreadPool::getMutex()
{
	return _thread_mutex;
}
