#include "Precompiled.h"
#include "UnitStateMove.h"

void UnitStateMove::Update(Unit * ownUnit, float deltaTime)
{
	ownUnit->SetAnimation(2);
}

bool UnitStateMove::FindEnemyInRange(Unit * ownUnit, std::shared_ptr<Terrain> terrain, std::vector<std::weak_ptr<RTSObject>>& mEnemy, UnitCommand command)
{
	return false;
}
