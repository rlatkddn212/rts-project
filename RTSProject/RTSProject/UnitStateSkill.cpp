#include "Precompiled.h"
#include "UnitStateSkill.h"

void UnitStateSkill::Update(Unit * ownUnit, float deltaTime)
{
}

bool UnitStateSkill::FindEnemyInRange(Unit * ownUnit, std::shared_ptr<Terrain> terrain, std::vector<std::weak_ptr<RTSObject>>& mEnemy, UnitCommand command)
{
	return false;
}
