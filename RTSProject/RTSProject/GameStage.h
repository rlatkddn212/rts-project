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
#include "MoveComponent.h"
#include "MiniMap.h"
#include "Player.h"
#include "GamePlayer.h"

class GamePlayer;
class Player;
class GameStage : public WindowGroup
{
public:
											GameStage();
	virtual								   ~GameStage();

	virtual void							Initialize(GLFWwindow* window, int w, int h);
	virtual void							Terminate();
	virtual void							Update(float deltaTime);
	virtual void							AddRender();

	virtual void							PressKey(bool* keys);
	virtual void							CursorPos(double xPos, double yPos);
	virtual void							MouseButton(int button, int action);
	virtual void							MouseWheel(double yPos);

private:
	std::shared_ptr<GamePlayer>				mGamePlayers;
	std::vector<std::shared_ptr<Player>>	mPlayers;

	std::shared_ptr<MoveComponent>			mMoveController;
	std::vector<std::shared_ptr<Building>>	mBuildings;
	std::shared_ptr<HeightMap>				mHeightMap;
	std::shared_ptr<Terrain>				mTerrain;
	std::shared_ptr<AxisObject>				mAxis;
	std::shared_ptr<MiniMap>				mMiniMap;

	bool									mIsLeftPress;
};

