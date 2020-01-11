#include "Precompiled.h"
#include "UnitInfo.h"

UnitInfo::UnitInfo()
{
	type = 0;
	health = 100;
	attackspeed = 10.0;
	range = 20.0;
	defense = 5.0;
	speed = 3.0;
	damege = 10.0;
}

UnitInfo::~UnitInfo()
{
}

std::string UnitInfo::GetModel()
{
	return "Assets/Model/magician.X";
}
