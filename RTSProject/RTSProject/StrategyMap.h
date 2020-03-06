/**
 * StrategyMap
 * 맵 영역에 적들의 위치, 동맹의 위치 값들을 저장해 두고 AI가 사용할 수 있게 한다.
 */

#pragma once
#include "Precompiled.h"
#include <vector>
#include <set>
#include "terrain.h"
#include "player.h"
#include "mapobject.h"

enum AREASTATE
{
	STATE_UNKNOWN,
	STATE_FRIENDLY,
	STATE_ENEMY,
	STATE_CONFLICT
};

struct Area
{
											Area(SDL_FRect mArea);
	void									Update(std::shared_ptr<Terrain> terrain, int teamNo);
	bool									Connected(std::shared_ptr<Terrain> terrain, glm::ivec2 from);
	glm::ivec2								GetCenter();
	
	SDL_FRect								mMapArea;
	AREASTATE								mState;

	int										mEnemyArea;
	int										mFriendlyArea;
	int										mValue;
};

class StrategyMap
{
public:
											StrategyMap(std::shared_ptr<Terrain> terrain, std::shared_ptr<Player> player, glm::ivec2 numAreas);
										   ~StrategyMap();

	void									Update(std::set<std::shared_ptr<Unit>> &enemies);

	std::shared_ptr<Area>					GetScoutArea(glm::ivec2 from);
	std::shared_ptr<Area>					GetAttackArea(glm::ivec2 from);
	std::shared_ptr<Area>					GetRandomArea();

private:
	std::shared_ptr<Terrain>				mTerrain;
	std::shared_ptr<Player>					mPlayer;
	glm::ivec2								mSize;
	int *									mInfluenceMap;
	std::vector<Area>						mAreas;
};