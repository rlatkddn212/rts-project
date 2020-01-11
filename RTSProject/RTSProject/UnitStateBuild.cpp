#include "Precompiled.h"
#include "UnitStateBuild.h"

void UnitStateBuild::Update(Unit * ownUnit, float deltaTime)
{
}

bool UnitStateBuild::FindEnemyInRange(Unit * ownUnit, std::shared_ptr<Terrain> terrain, std::vector<std::weak_ptr<RTSObject>>& mEnemy, UnitCommand command)
{
	return false;
}
