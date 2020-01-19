#pragma once
#include "Player.h"

enum PlayerState
{
	PLAYER_NONE,
	PLAYER_UNIT_COMMAND,
	PLAYER_UNIT_SELECTED,
	PLAYER_BUILDING,
};

enum CommandState
{
	PressA,
	PressP,
	PressQ,
	PressW,
	PressE,
	PressR,
};
class Player;
class GamePlayer : public Player
{
public:
											GamePlayer(std::shared_ptr<Mouse> mouse);
	virtual								   ~GamePlayer();

	virtual void							Initialize(std::shared_ptr<Terrain> terrain, int w, int h);
	virtual void							Terminate();
	virtual void							Update(float deltaTime);
	virtual void							AddRender(std::shared_ptr<Camera> camera);

	virtual void							PressKey(bool* keys);
	virtual void							CursorPos(std::shared_ptr<Camera> camera, double xPos, double yPos);
	virtual void							MouseButton(std::shared_ptr<Camera> camera, int button, int action);
	virtual void							MouseWheel(std::shared_ptr<Camera> camera, double yPos);

	bool									PressUnitCommand(bool * keys);
	void									NoneState();
	bool									IsSelectUnit(std::shared_ptr<Camera> camera);
	void									SelectUnitInRect(std::shared_ptr<Camera> camera);

private:
	bool									CreateBuilding(std::shared_ptr<Camera> camera);
	void									BuildingToPlace(int type);
	void									CommandUnit(std::shared_ptr<Camera> camera);
	std::shared_ptr<RTSObject>				IsSelectEmeryUnit(std::shared_ptr<Camera> camera);

protected:

	// 부대지정
	std::vector<std::shared_ptr<RTSObject>> mUnitGroup[10];
	std::vector<std::shared_ptr<Unit>>		mSelectedUnit;

	std::shared_ptr<Mouse>					mMouse;
	int										mMouseX;
	int										mMouseY;
	PlayerState								mPlayerState;
	CommandState							mCommandState;
};

