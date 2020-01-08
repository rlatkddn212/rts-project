#pragma once

#include <vector>
#include "building.h"
#include "unit.h"
#include "Player.h"

class AIAction
{
public:
	virtual std::shared_ptr<AIAction>		RequiresAction(std::shared_ptr<Player> player) = 0;
	virtual void							PerformAction(std::shared_ptr<Player> player) = 0;
};

// 훈련
class TrainUnitAction : public AIAction
{
public:
											TrainUnitAction(int unitType);
	std::shared_ptr<AIAction>				RequiresAction(std::shared_ptr<Player> player);
	void									PerformAction(std::shared_ptr<Player> player);
 
private:
	int										mTrainUnit;
};

// 건물 건설
class ConstructBuildingAction : public AIAction
{
public:
											ConstructBuildingAction(int buildingType);
	std::shared_ptr<AIAction>				RequiresAction(std::shared_ptr<Player> player);
	void									PerformAction(std::shared_ptr<Player> player);

private:
	int										mConstructBuilding;
};
