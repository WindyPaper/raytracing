#ifndef _THREAD_POOL_
#define _THREAD_POOL_

#include <thread>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "Camera.h"
#include "World.h"
#include "ThreadLocalStorge.h"

class ThreadInfo;

struct TaskInfo
{
	int offsetx, offsety;
	int tile_w, tile_h;

	//std::thread::id thread_id;
	World *w;

	TaskInfo(int ox, int oy, int w, int h, World *pW) :
		offsetx(ox), offsety(oy), tile_w(w), tile_h(h), w(pW)
	{
	}

	~TaskInfo()
	{
	}
};

class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();

	int initThreads();

	void addTask(TaskInfo *info);
	TaskInfo *aquireTask();

	int getMachineProcessorNum() const;
	std::condition_variable &getConditionEvent();
	std::mutex &getMutex();

private:
	std::mutex _thread_mutex;
	std::condition_variable _condition_event;

	std::unordered_map<ThreadInfo*, int> _thread_map;
	std::vector<TaskInfo*> _tasks;
};

struct ThreadInfo
{
	RenderTLS tls;
	ThreadPool *pThreadPool;
	std::thread *pThread;
	std::atomic<bool> is_stop;

	ThreadInfo(ThreadPool *pTp) :
		pThreadPool(pTp),
		pThread(NULL),
		is_stop(false)
	{}

	void stopThread()
	{
		is_stop = true;
		pThreadPool->getConditionEvent().notify_all();
		pThread->join();
	}

	void startThread()
	{
		pThread = new std::thread(
			[=] {
			TaskInfo *taskInfo = NULL;
			while (!is_stop)
			{
				taskInfo = pThreadPool->aquireTask();

				if (taskInfo == NULL)
				{
					std::mutex &lock = pThreadPool->getMutex();
					std::condition_variable &condition_event = pThreadPool->getConditionEvent();
					std::unique_lock<std::mutex> local_lock(lock);
					condition_event.wait(local_lock);
				}				
				else
				{
					taskInfo->w->camera_ptr->render_scene(&tls, *taskInfo->w, taskInfo->offsetx, taskInfo->offsety, taskInfo->tile_w, taskInfo->tile_h);

					delete taskInfo;
				}				
			}			
		}
		);
		//pThread->detach();
		//thread_id = t.get_id();
	}
};

#endif
