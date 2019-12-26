#pragma once

#include "Terrain.h"
#include "Unit.h"
class MoveController
{
	void									Update(float deltaTime);


	std::shared_ptr<Terrain>				mTerrain;
	std::vector<std::shared_ptr<Unit> >		mUnits;
};

