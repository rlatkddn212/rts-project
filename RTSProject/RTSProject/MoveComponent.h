/**
 * MoveComponent
 * 이동가능한 Object에 대한 컴포넌트
 */
#pragma once
#include "Terrain.h"
#include "Unit.h"

class Unit;
class MoveComponent
{
public:
											MoveComponent();
										   ~MoveComponent();

	void									SetTerrain(std::shared_ptr<Terrain> terrain);
	void									SetUnit(std::shared_ptr<Unit> unit);
	void									Update(float deltaTime);
	glm::vec2								MoveUnit(std::shared_ptr<Unit> unit, float len);
	void									Arrive();

private:
	std::shared_ptr<Terrain>				mTerrain;
	std::weak_ptr<Unit>						mUnit;
};