#include "Precompiled.h"
#include "RTSObjectInfo.h"

RTSObjectInfo::RTSObjectInfo()
{
	type = 0;
	health = 100;
	attackspeed = 10.0;
	range = 10.0;
	defense = 5.0;
	speed = 3.0;
	damege = 10.0;
	sight = 20.0;
}

RTSObjectInfo::~RTSObjectInfo()
{
}

std::string RTSObjectInfo::GetModel()
{
	return "Assets/Model/magician.X";
}
