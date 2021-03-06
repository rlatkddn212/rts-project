#include "Precompiled.h"
#include "ObjectResourcePool.h"

using namespace std;

/**
 * ObjectResourcePool �ʱ�ȭ
 */
void ObjectResourcePool::Initialize()
{
	std::string s = "Assets/Model/magician.X";

	for (size_t i = 0; i < 3; ++i)
	{
		mUnits.push_back(make_shared<Unit>());
		mUnits[i]->MakeModel(s);
	}

	for (size_t i = 0; i < 3; ++i)
	{
		mBuildings.push_back(make_shared<Building>());
		mBuildings[i]->MakeModel(i);
	}
	
	for (size_t i = 0; i < 2; ++i)
	{
		mMapObjects.push_back(make_shared<MapObject>());
		mMapObjects[i]->MakeModel(i);
	}
	
	mMiniMap = make_shared<MiniMap>();
	mMiniMap->MakeModel();
	mAxisObject = make_shared<AxisObject>(0);
	mAxisObject->MakeModel();
}

void ObjectResourcePool::Load()
{
}
