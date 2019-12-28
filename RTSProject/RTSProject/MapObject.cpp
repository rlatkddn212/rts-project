#include "Precompiled.h"
#include "MapObject.h"

MapObject::MapObject(int type)
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

MapObject::~MapObject()
{
}

void MapObject::Update(float deltaTime)
{
	mStaticMesh->Update(deltaTime);
}

void MapObject::Render(std::shared_ptr<Camera> camera)
{
	mStaticMesh->RenderModel(camera);
}
