#include "ThreadPool.h"

ThreadPool::ThreadPool()
{

}

ThreadPool::~ThreadPool()
{
	for (auto i = _task_threads.begin(); i != _task_threads.end(); ++i)
	{
		delete (*i);
	}
	_task_threads.clear();
}

void ThreadPool::addTask(TaskInfo *info)
{
	info->startThread();
	_task_threads.push_back(info);
	//_task_threads.back().startThread();
	//_task_threads[_task_threads.size() - 1].startThread();
}
