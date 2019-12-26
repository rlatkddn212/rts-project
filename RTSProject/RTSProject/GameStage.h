#pragma once
#include "WindowGroup.h"
#include "HeightMap.h"
#include "Terrain.h"
#include "StaticMesh.h"
#include "Camera.h"
#include "Mouse.h"
#include "SkinnedMesh.h"
#include "AxisObject.h"
#include "Unit.h"
#include "Building.h"
#include "MoveController.h"

enum PlayerState
{
	PLAYER_NONE,
	PLAYER_UNIT_SELECTED,
	PLAYER_BUILDING,
};

class GameStage : public WindowGroup
{
public:
	GameStage();
	virtual								   ~GameStage();

	virtual void							Initialize(GLFWwindow* window, int w, int h);
	virtual void							Terminate();
	virtual void							Update(float deltaTime);
	virtual void							Render();

	virtual void							PressKey(bool* keys);
	virtual void							CursorPos(double xPos, double yPos);
	virtual void							MouseButton(int button, int action);
	virtual void							MouseWheel(double yPos);


	std::shared_ptr<MoveController>			mMoveController;
	std::vector<std::shared_ptr<Building>>	mBuildings;
	std::shared_ptr<Building>				mBuildingToPlace;
	std::shared_ptr<HeightMap>				mHeightMap;
	std::shared_ptr<Terrain>				mTerrain;
	std::vector<std::shared_ptr<BoxObject>> mBox;
	std::vector<std::shared_ptr<Unit> >		mUnits;
	std::shared_ptr<AxisObject>				mAxis;

	bool									mIsLeftPress;
	PlayerState								playerState;
};

