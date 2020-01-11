#pragma once
#include "Unit.h"
#include "Terrain.h"

class Unit;
enum UnitCommand;
// 상태 패턴
class UnitState
{
public:
											UnitState() {}
	virtual								   ~UnitState() {}

	virtual void							Update(Unit * ownUnit, float deltaTime) = 0;
	virtual bool							FindEnemyInRange(Unit* ownUnit, std::shared_ptr<Terrain> terrain, std::vector<std::weak_ptr<RTSObject> >& mEnemy, UnitCommand command) = 0;
};
