#include "Precompiled.h"
#include "UnitStateDie.h"

void UnitStateDie::Update(Unit * ownUnit, float deltaTime)
{
	ownUnit->SetAnimation(4);
}

bool UnitStateDie::FindEnemyInRange(Unit * ownUnit, std::shared_ptr<Terrain> terrain, std::vector<std::weak_ptr<RTSObject>>& mEnemy, UnitCommand command)
{
	return false;
}
