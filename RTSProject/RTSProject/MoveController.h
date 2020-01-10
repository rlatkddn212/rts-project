#pragma once
#include "Precompiled.h"
#include "Terrain.h"
#include "Unit.h"

class Unit;
class MoveController
{
public:
											MoveController();
										   ~MoveController();

	void									SetTerrain(std::shared_ptr<Terrain> terrain);
	void									SetUnit(std::shared_ptr<Unit> unit);
	void									Update(float deltaTime);
	glm::vec2								MoveUnit(std::shared_ptr<Unit> unit, float len);
	void									Arrive();

private:
	std::shared_ptr<Terrain>				mTerrain;
	std::shared_ptr<Unit>					mUnit;
};