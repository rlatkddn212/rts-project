#pragma once
#include "Player.h"

enum PlayerState
{
	PLAYER_NONE,
	PLAYER_UNIT_COMMAND,
	PLAYER_UNIT_SELECTED,
	PLAYER_BUILDING,
};

class GamePlayer : public Player
{
public:
											GamePlayer();
	virtual								   ~GamePlayer();

	virtual void							Initialize(std::shared_ptr<Terrain> terrain, int w, int h);
	virtual void							Terminate();
	virtual void							Update(float deltaTime);
	virtual void							Render(std::shared_ptr<Camera> camera);

	virtual void							PressKey(bool* keys);
	virtual void							CursorPos(std::shared_ptr<Mouse> mouse, std::shared_ptr<Camera> camera, double xPos, double yPos);
	virtual void							MouseButton(std::shared_ptr<Mouse> mouse, std::shared_ptr<Camera> camera, int button, int action);
	virtual void							MouseWheel(std::shared_ptr<Mouse> mouse, std::shared_ptr<Camera> camera, double yPos);

	bool									PressUnitCommand(bool * keys);
	void									NoneState();
	bool									IsSelectUnit(std::shared_ptr<Mouse> mouse, std::shared_ptr<Camera> camera);
	void									SelectUnit(std::shared_ptr<Mouse> mouse, std::shared_ptr<Camera> camera);

private:
	bool									CreateBuilding(std::shared_ptr<Mouse> mouse, std::shared_ptr<Camera> camera);
	void									BuildingToPlace(int type);

protected:

	// 부대지정
	std::vector<std::shared_ptr<RTSObject>> mUnitGroup[10];
	std::vector<std::shared_ptr<Unit>>		mSelectedUnit;

	int										mMouseX;
	int										mMouseY;
	PlayerState								playerState;
};

