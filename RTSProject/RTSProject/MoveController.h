#pragma once
#include "Precompiled.h"
#include "Terrain.h"
#include "Unit.h"
class MoveController
{
public:
											MoveController();
										   ~MoveController();

	void									SetTerrain(std::shared_ptr<Terrain> terrain);
	void									SetUnit(std::vector<std::shared_ptr<Unit> >& units);
	void									Update(float deltaTime);
	glm::vec2								MoveUnit(std::shared_ptr<Unit> unit, float len);

private:
	std::shared_ptr<Terrain>				mTerrain;
	std::vector<std::shared_ptr<Unit> >*	mpUnits;
};