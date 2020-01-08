#include "Precompiled.h"
#include "strategyMap.h"

Area::Area(SDL_FRect mArea)
{
}

void Area::Update(std::shared_ptr<Terrain> terrain, int teamNo)
{
}

bool Area::Connected(std::shared_ptr<Terrain> terrain, glm::ivec2 from)
{
	return false;
}

glm::ivec2 Area::GetCenter()
{
	return glm::ivec2();
}

StrategyMap::StrategyMap(std::shared_ptr<Terrain> _terrain, std::shared_ptr<Player> _player, glm::ivec2 numAreas)
{
}

StrategyMap::~StrategyMap()
{
}

void StrategyMap::Update(std::set<std::shared_ptr<Unit>>& enemies)
{
}

std::shared_ptr<Area> StrategyMap::GetScoutArea(glm::ivec2 from)
{
	return nullptr;
}

std::shared_ptr<Area> StrategyMap::GetAttackArea(glm::ivec2 from)
{
	return nullptr;
}

std::shared_ptr<Area> StrategyMap::GetRandomArea()
{
	return nullptr;
}
