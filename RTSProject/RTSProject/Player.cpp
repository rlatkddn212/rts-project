#include "Precompiled.h"
#include "Player.h"
using namespace std;

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(shared_ptr<Terrain> terrain, int w, int h)
{
	mTerrain = terrain;
	// 건물
	mBuildingToPlace = nullptr;

	// 유닛 배치
	for (int i = 0; i < 10; ++i)
	{
		int x = rand() % 100;
		int y = rand() % 100;
		shared_ptr<Unit> mesh = make_shared<Unit>(make_shared<UnitInfo>());
		mUnits.push_back(mesh);
		mUnits[i]->SetPosOnTerrain(mTerrain, glm::ivec2(x, y));
		mUnits[i]->AttachMoveComponent(terrain);
	}
}

void Player::Terminate()
{
}

void Player::Update(float deltaTime)
{
	for (int i = 0; i < 10; ++i)
	{
		mUnits[i]->Update(deltaTime);
		glm::vec3 unitPos = mUnits[i]->GetPosition();
		mUnits[i]->SetPosOnTerrain(mTerrain, glm::vec2(unitPos.x, -unitPos.z));
	}
}

void Player::Render(std::shared_ptr<Camera> camera)
{
	for (int i = 0; i < 10; ++i)
	{
		mUnits[i]->Render(camera);
	}

	if (mBuildingToPlace)
	{
		mBuildingToPlace->Render(camera);
	}

	for (int i = 0; i < mBuildings.size(); ++i)
	{
		mBuildings[i]->Render(camera);
	}
}

void Player::SetPosOnTerrain(shared_ptr<Texture> fog)
{
	for (int i = 0; i < 10; ++i)
	{
		mUnits[i]->SetFogTexture(fog);
	}
}

void Player::SetFogTexture(std::shared_ptr<Texture> fog)
{
	for (int i = 0; i < 10; ++i)
	{
		mUnits[i]->SetFogTexture(fog);
	}
}

