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
	for (int i = 0; i < 5; ++i)
	{
		int x = rand() % 100;
		int y = rand() % 100;
		shared_ptr<Unit> mesh = make_shared<Unit>(make_shared<RTSObjectInfo>());
		mUnits.push_back(mesh);
		mUnits[i]->InitPosOnTerrain(mTerrain, glm::ivec2(x, y));
		
		shared_ptr<MoveController> mMoveComponent = make_shared<MoveController>();
		mMoveComponent->SetTerrain(terrain);
		mMoveComponent->SetUnit(mUnits[i]);
		mUnits[i]->AttachMoveComponent(mMoveComponent);
	}
}

void Player::Terminate()
{
}

void Player::Update(float deltaTime)
{
	for (int i = 0; i < mUnits.size(); ++i)
	{
		if (mUnits[i]->FindEnemyInRange(mTerrain, mEmeryUnits))
		{

		}
	}

	for (int i = 0; i < mUnits.size(); ++i)
	{
		mUnits[i]->Update(deltaTime);
	}
}

void Player::Render(std::shared_ptr<Camera> camera)
{
	for (int i = 0; i < mUnits.size(); ++i)
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
	for (int i = 0; i < mUnits.size(); ++i)
	{
		mUnits[i]->SetFogTexture(fog);
	}
}

void Player::SetFogTexture(std::shared_ptr<Texture> fog)
{
	for (int i = 0; i < mUnits.size(); ++i)
	{
		mUnits[i]->SetFogTexture(fog);
	}
}

void Player::SetEnemy(std::vector<std::shared_ptr<RTSObject>>& enemy)
{
	mEmeryUnits.clear();
	mEmeryUnits.reserve(enemy.size());
	for (int i = 0; i < enemy.size(); ++i)
	{
		mEmeryUnits.push_back(enemy[i]);
	}
}

