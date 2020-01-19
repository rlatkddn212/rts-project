/**
 * TODO : Main 쓰레드와 랜더 쓰레드 두 쓰레드에서 접근하도록 코드 변경 필요
 */

#include "Precompiled.h"
#include "RenderManager.h"

void RenderManager::Initialize()
{
	mSize = 10000;
	mCount = 0;
}

void RenderManager::AddQueue(std::shared_ptr<RenderObject> obj)
{
	std::unique_lock<std::mutex> lk(mMutex);
	cv.wait(lk, [&]() { return mCount < mSize; });
	q.push(obj);
	mCount++;
	lk.unlock();
	cv.notify_one();
}

std::vector<std::shared_ptr<RenderObject>> RenderManager::GetQueue()
{
	std::vector<std::shared_ptr<RenderObject>> ret;
	std::unique_lock<std::mutex> lk(mMutex);
	cv.wait(lk, [&]() { return mCount != 0; });
	
	while (!q.empty())
	{
		ret.push_back(q.front());
		q.pop();
		mCount--;
	}

	lk.unlock();
	cv.notify_one();

	return ret;
}

void RenderManager::Render()
{
	std::vector<std::shared_ptr<RenderObject>> renderData = GetQueue();
	for (int i = 0; i < renderData.size(); ++i)
	{
		renderData[i]->UpdateModel();
		renderData[i]->Render(renderData[i]->mCamera);
	}
}