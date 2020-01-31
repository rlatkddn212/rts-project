#include "Precompiled.h"
#include "Building.h"
#include "ObjectResourcePool.h"

using namespace std;

Building::Building(int type)
{
	mRenderState = DeferedRendering;
	shared_ptr<Building> sourceUnit = ObjectResourcePool::GetInstance()->GetBuilding(type);

	mBuildingMesh = sourceUnit->GetMesh();
	mType = type;
}

Building::~Building()
{
}

void Building::MakeModel(int type)
{
	mBuildingMesh = std::make_shared<BuildingMesh>();
	mType = type;

	if (type == 0)
	{
		mBuildingMesh->LoadModel("Assets/Model/barracks.X");
	}
	else if (type == 1)
	{
		mBuildingMesh->LoadModel("Assets/Model/tower.x");
	}
	else if (type == 2)
	{
		mBuildingMesh->LoadModel("Assets/Model/townhall.x");
	}

	mBuildingMesh->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

bool Building::isPossibleBuild(shared_ptr<Terrain> terrain, int x, int y)
{
	glm::vec2 size = terrain->GetSize();
	// 건물마다 다르겠지만 우선 3x3 칸을 사용
	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			int nx = x + i - 1;
			int ny = y + j - 1;
			glm::ivec2 newPos = glm::ivec2(nx, ny);

			bool isBuild = false;

			if (0 <= nx && nx < size.x && 0 <= ny && ny < size.y)
			{
				if (terrain->IsMovableTile(newPos)
					&& ! terrain->IsObjectOnTile(newPos))
				{
					isBuild = true;
				}
			}
			
			if (!isBuild)
			{
				return false;
			}
		}
	}

	return true;
}

void Building::BuildOnTerrain(std::shared_ptr<Terrain> terrain, int x, int y)
{
	mBuildingMesh->SetPosition(glm::vec3(x, terrain->GetHeight(glm::vec2(x, -y)), -y));

	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			int nx = x + i - 1;
			int ny = y + j - 1;

			terrain->SetTileState(glm::ivec2(x, y), TileState::StaticObject);
		}
	}
}

void Building::RenderShadow(std::shared_ptr<Camera> camera)
{
}

void Building::Update(float deltaTime)
{
	mBuildingMesh->Update(deltaTime);
}

void Building::Render(std::shared_ptr<Camera> camera)
{
	mBuildingMesh->SetPosition(mPos);
	//mBuildingMesh->SetRotation(mRot);
	//mBuildingMesh->SetScale(mSca);
	mBuildingMesh->RenderModel(camera);
}

void Building::AddRender(std::shared_ptr<Camera> camera)
{
	shared_ptr<Building> ro = make_shared<Building>(*this);
	ro->mCamera = camera;
	RenderManager::GetInstance()->AddQueue(ro);
}
