#pragma once
#include "Precompiled.h"
#include "Building.h"
#include "Unit.h"
#include "Camera.h"

class Player
{
public:
											Player();
	virtual								   ~Player();

	virtual void							Initialize(std::shared_ptr<Terrain> terrain, int w, int h);
	virtual void							Terminate();
	virtual void							Update(float deltaTime);
	virtual void							Render(std::shared_ptr<Camera> camera);

	void									AddBuilding(std::shared_ptr<Building> building) { mBuildings.push_back(building); }
	void									AddUnit(std::shared_ptr<Unit> unit) { mUnits.push_back(unit); }
	std::vector<std::shared_ptr<Building>>	GetBuilding() { return mBuildings; }
	std::vector<std::shared_ptr<Unit> >		GetUnit() { return mUnits; }
	void									SetPosOnTerrain(std::shared_ptr<Texture> fog);
	void									SetFogTexture(std::shared_ptr<Texture> fog);

protected:

	int										mWidth;
	int										mHeight;
	std::shared_ptr<Building>				mBuildingToPlace;
	std::shared_ptr<Terrain>				mTerrain;
	std::vector<std::shared_ptr<Building>>	mBuildings;
	std::vector<std::shared_ptr<Unit> >		mUnits;
};