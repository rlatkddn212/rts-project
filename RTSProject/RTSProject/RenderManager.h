#pragma once
#include "Singleton.h"
#include "RenderObject.h"
#include "Camera.h"
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>

class RenderManager : public Singleton<RenderManager>
{
public:
	void									Initialize();
	void									AddQueue(std::shared_ptr<RenderObject> obj);
	std::vector<std::shared_ptr<RenderObject>>	GetQueue();
	void									Render();


private:
	std::condition_variable					cv;
	std::mutex								mMutex;
	std::queue<std::shared_ptr<RenderObject>> q;

	int										mSize;
	int										mCount;
};
