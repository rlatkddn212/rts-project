#pragma once
#include "UnitState.h"
class Unit;
class UnitStateAttack : public UnitState
{
public:
	virtual void							Update(Unit * ownUnit, float deltaTime);
	virtual bool							FindEnemyInRange(Unit* ownUnit, std::shared_ptr<Terrain> terrain, std::vector<std::weak_ptr<RTSObject> >& mEnemy, UnitCommand command);
};