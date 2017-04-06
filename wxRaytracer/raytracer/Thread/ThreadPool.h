#ifndef _THREAD_POOL_
#define _THREAD_POOL_

#include <thread>
#include <functional>
#include <unordered_map>

#include "Camera.h"
#include "World.h"
#include "ThreadLocalStorge.h"


struct TaskInfo
{
	int offsetx, offsety;
	int tile_w, tile_h;

	World *pWorld;
	RenderTLS tls;
	
	//std::thread::id thread_id;
	int tile_num;

	TaskInfo(int ox, int oy, int w, int h, World *pW) :
		offsetx(ox), offsety(oy), tile_w(w), tile_h(h), pWorld(pW)
	{
		//seperate as tile
		
	}

	~TaskInfo()
	{
	}

	void startThread()
	{
		std::thread t = std::thread(
		[=] {
			//RenderTLS *pTls = new RenderTLS;
			this->pWorld->camera_ptr->render_scene(&tls, *pWorld, offsetx, offsety, tile_w, tile_h);
		}
		);
		t.detach();
		//thread_id = t.get_id();
	}
};

class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();

	void addTask(TaskInfo *info);

private:
	std::unordered_map<std::thread::id, int> _thread_map;
	std::vector<TaskInfo*> _task_threads;
};

#endif
