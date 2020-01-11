#pragma once
#include "UnitState.h"
class Unit;
class UnitStateSkill : public UnitState
{
public:
	virtual void							Update(Unit * ownUnit, float deltaTime);
	virtual bool							FindEnemyInRange(Unit* ownUnit, std::shared_ptr<Terrain> terrain, std::vector<std::weak_ptr<RTSObject> >& mEnem, UnitCommand command);
};

