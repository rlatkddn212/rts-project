#pragma once

#include "Singleton.h"
#include "Building.h"
#include "Unit.h"
#include "BoxObject.h"
#include "MiniMap.h"
#include "AxisObject.h"

class ObjectResourcePool : public Singleton<ObjectResourcePool>
{
public:
	void									Initialize();
	void									Load();

	std::shared_ptr<Unit>					GetUnit(int type) { return mUnits[type]; }
	std::shared_ptr<Building>				GetBuilding(int type) { return mBuildings[type]; }
	std::shared_ptr<MapObject>				GetMapObject(int type) { return mMapObjects[type]; }
	std::shared_ptr<MiniMap>				GetMiniMap() { return mMiniMap; }
	std::shared_ptr<AxisObject>				GetAxisObject() { return mAxisObject; }

private:
	std::vector<std::shared_ptr<Unit>>		mUnits;
	std::vector<std::shared_ptr<Building>>	mBuildings;

	std::vector<std::shared_ptr<MapObject>>	mMapObjects;
	std::shared_ptr<MiniMap>				mMiniMap;
	std::shared_ptr<AxisObject>				mAxisObject;
	std::shared_ptr<BoxObject>				mBoxObject;
};