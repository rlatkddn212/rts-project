#include "Precompiled.h"
#include "MapObject.h"
#include "ObjectResourcePool.h"

MapObject::MapObject(int type)
{
	std::shared_ptr<MapObject> mini = ObjectResourcePool::GetInstance()->GetMapObject(type);
	mStaticMesh = mini->mStaticMesh;
}

MapObject::~MapObject()
{
}

void MapObject::MakeModel(int type)
{
	mStaticMesh = std::make_shared<StaticMesh>();
	if (type == 0)
	{
		mStaticMesh->LoadModel("Assets/Model/tree.x");
	}
	else
	{
		mStaticMesh->LoadModel("Assets/Model/stone.x");
	}
}

void MapObject::Update(float deltaTime)
{
	mStaticMesh->Update(deltaTime);
}

void MapObject::Render(std::shared_ptr<Camera> camera)
{
	mStaticMesh->RenderModel(camera);
}

void MapObject::AddRender(std::shared_ptr<Camera> camera)
{
	std::shared_ptr<MapObject> ro = std::make_shared<MapObject>(*this);
	ro->mCamera = camera;
	RenderManager::GetInstance()->AddQueue(ro);
}
