#pragma once
#include <vector>
#include "GroupAI.h"
#include "Player.h"
#include "StrategyMap.h"

class MasterAI
{
public:
											MasterAI(std::shared_ptr<Player> player, std::shared_ptr<Terrain> terrain);
										   ~MasterAI();

	void									Update(float deltaTime);
	void									HandleGroups();
	void									DoMasterAI();

	void									BuildManager();
	void									StrategicManager();

	void									EnemiesSpotted(std::vector<std::shared_ptr<Unit>> &manyEnemies);

private:
	std::shared_ptr<Player>					mPlayer;
	std::shared_ptr<Terrain>				mTerrain;
	std::shared_ptr<StrategyMap>			mStrategyMap;

	GroupAI									mUnitPool;
	std::vector<std::shared_ptr<GroupAI>>	mGroups;
	std::vector<std::shared_ptr<Unit>>		mVisibleEnemies;

	float									mNextGroupUpdate;
	float									mNextUpdate;
	SDL_FRect								mBase;
};