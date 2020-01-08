#include "Precompiled.h"
#include "Action.h"
#include "Player.h"
#include "StrategyMap.h"

TrainUnitAction::TrainUnitAction(int unitType)
{
}

std::shared_ptr<AIAction> TrainUnitAction::RequiresAction(std::shared_ptr<Player> player)
{
	return nullptr;
}

void TrainUnitAction::PerformAction(std::shared_ptr<Player> player)
{
}

ConstructBuildingAction::ConstructBuildingAction(int buildingType)
{
}

std::shared_ptr<AIAction> ConstructBuildingAction::RequiresAction(std::shared_ptr<Player> player)
{
	return nullptr;
}

void ConstructBuildingAction::PerformAction(std::shared_ptr<Player> player)
{
}
