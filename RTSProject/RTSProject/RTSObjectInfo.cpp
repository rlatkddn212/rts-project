#include "Precompiled.h"
#include "RTSObjectInfo.h"

RTSObjectInfo::RTSObjectInfo()
{
	mType = 0;
	health = 100;
	attackspeed = 0.6;
	range = 10.0;
	defense = 5.0;
	speed = 3.0;
	damege = 10.0;
	sight = 20.0;
}

RTSObjectInfo::~RTSObjectInfo()
{
}

int RTSObjectInfo::GetModel()
{
	return mType;
}
