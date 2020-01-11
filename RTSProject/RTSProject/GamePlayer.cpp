#include "Precompiled.h"
#include "GamePlayer.h"

using namespace std;

GamePlayer::GamePlayer(shared_ptr<Mouse> mouse)
{
	mMouse = mouse;
}

GamePlayer::~GamePlayer()
{
}

void GamePlayer::Initialize(shared_ptr<Terrain> terrain, int w, int h)
{
	Player::Initialize(terrain, w, h);

	for (int i = 0; i < mUnits.size(); ++i)
	{
		mUnits[i]->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}

	mWidth = w;
	mHeight = h;
	mPlayerState = PLAYER_NONE;
}

void GamePlayer::Terminate()
{
}

void GamePlayer::Update(float deltaTime)
{
	Player::Update(deltaTime);
}

void GamePlayer::Render(std::shared_ptr<Camera> camera)
{
	Player::Render(camera);
}

bool GamePlayer::PressUnitCommand(bool * keys)
{
	// 유닛 명령
	if (keys[GLFW_KEY_A] == true)
	{
		mMouse->SetCursorImage(1);
		mCommandState = PressA;
	}
	else if (keys[GLFW_KEY_S] == true)
	{
		for (int i = 0; i < mSelectedUnit.size(); ++i)
		{
			mSelectedUnit[i]->SetStopCommand();
		}

		return false;
	}
	else if (keys[GLFW_KEY_H] == true)
	{
		for (int i = 0; i < mSelectedUnit.size(); ++i)
		{
			mSelectedUnit[i]->SetHoldCommand();
		}

		return false;
	}
	else if (keys[GLFW_KEY_P] == true)
	{
		mCommandState = PressP;
	}
	// 스킬
	else if (keys[GLFW_KEY_Q] == true)
	{

	}
	else if (keys[GLFW_KEY_W] == true)
	{

	}
	else if (keys[GLFW_KEY_E] == true)
	{

	}
	else if (keys[GLFW_KEY_R] == true)
	{

	}
	else
	{
		return false;
	}

	return true;
}

void GamePlayer::PressKey(bool * keys)
{
	if (mPlayerState == PLAYER_UNIT_SELECTED)
	{
		if (PressUnitCommand(keys))
		{
			mPlayerState = PLAYER_UNIT_COMMAND;
		}
	}
	else
	{
		// 건물 위치 지정
		if (keys[GLFW_KEY_1] == true)
		{
			BuildingToPlace(0);
		}
		else if (keys[GLFW_KEY_2] == true)
		{
			BuildingToPlace(1);
		}
		else if (keys[GLFW_KEY_3] == true)
		{
			BuildingToPlace(2);
		}
	}

	if (keys[GLFW_KEY_ESCAPE] == true)
	{
		NoneState();
	}
}

void GamePlayer::CursorPos(std::shared_ptr<Camera> camera, double xPos, double yPos)
{
	mMouseX = xPos;
	mMouseY = yPos;
	
	if (mPlayerState == PLAYER_BUILDING)
	{
		Ray ray;
		ray.SetRay(camera, mMouseX, mMouseY);
		glm::ivec2 pos;

		if (mTerrain->Intersect(ray, pos))
		{
			if (mBuildingToPlace->isPossibleBuild(mTerrain, pos.x, pos.y))
			{
				mBuildingToPlace->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
			}
			else
			{
				mBuildingToPlace->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
			}

			mBuildingToPlace->SetPosition(glm::vec3(pos.x, mTerrain->GetHeight(glm::vec2(pos.x, -pos.y)), -pos.y));
		}
	}
}

void GamePlayer::MouseButton(std::shared_ptr<Camera> camera, int button, int action)
{
	// 오른쪽 마우스 클릭시 유닛 이동
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		mMouse->SetCursorImage(2);

		Ray ray;
		ray.SetRay(camera, mMouseX, mMouseY);
		glm::ivec2 pos;
		if (mTerrain->Intersect(ray, pos))
		{
			for (int i = 0; i < mUnits.size(); ++i)
			{
				if (mUnits[i]->IsSelected())
				{
					mUnits[i]->SetMove(mTerrain, pos);
				}
			}
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		mMouse->SetCursorImage(0);
	}
	// 왼쪽 마우스 클릭시
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		// 기본 상태일 경우 유닛 선택 확인
		if (mPlayerState == PLAYER_NONE | mPlayerState == PLAYER_UNIT_SELECTED)
		{
			if (IsSelectUnit(camera))
			{
				mPlayerState = PLAYER_UNIT_SELECTED;
				//mMouse->VisiableDragBox(true);
			}
			else
			{
				NoneState();
			}
		}
		// 건물 짓기 준비 상태일 경우
		else if (mPlayerState == PLAYER_BUILDING)
		{
			if (CreateBuilding(camera))
			{
				NoneState();
			}
		}
		else if (mPlayerState == PLAYER_UNIT_COMMAND)
		{
			CommandUnit(camera);
			mPlayerState = PLAYER_UNIT_SELECTED;
			mMouse->SetCursorImage(0);
		}
		else
		{
			NoneState();
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{

	}
}


void GamePlayer::CommandUnit(std::shared_ptr<Camera> camera)
{
	Ray ray;
	ray.SetRay(camera, mMouseX, mMouseY);
	glm::ivec2 pos;
	
	if (mTerrain->Intersect(ray, pos))
	{
		for (int i = 0; i < mSelectedUnit.size(); ++i)
		{
			if (mCommandState == PressA)
			{
				mSelectedUnit[i]->SetAttackCommand(mTerrain, pos);
			}
			else if (mCommandState == PressP)
			{
				mSelectedUnit[i]->SetPatrolCommand(mTerrain, pos);
			}
		}
	}
}

void GamePlayer::MouseWheel(std::shared_ptr<Camera> camera, double yPos)
{
}

bool GamePlayer::CreateBuilding(std::shared_ptr<Camera> camera)
{
	Ray ray;
	ray.SetRay(camera, mMouseX, mMouseY);
	glm::ivec2 pos;
	if (mTerrain->Intersect(ray, pos))
	{
		// 만약 건설 가능한 구역이라면 건물 추가
		if (mBuildingToPlace->isPossibleBuild(mTerrain, pos.x, pos.y))
		{
			shared_ptr<Building> building = make_shared<Building>(mBuildingToPlace->GetType());
			building->BuildOnTerrain(mTerrain, pos.x, pos.y);
			mBuildings.push_back(building);

			return true;
		}
	}

	return false;
}

void GamePlayer::SelectUnitInRect(std::shared_ptr<Camera> camera)
{
	for (int i = 0; i < mUnits.size(); ++i)
	{
		glm::vec2 p = mUnits[i]->GetScreenPos(camera);
		p.x = p.x * (mWidth / 2) + mWidth / 2;
		p.y = -p.y * (mHeight / 2) + mHeight / 2;

		if (mMouse->IsDragBoxPos(p))
		{
			mPlayerState = PLAYER_UNIT_SELECTED;
			mUnits[i]->Select();
			mSelectedUnit.push_back(mUnits[i]);
		}
	}
}

bool GamePlayer::IsSelectUnit(std::shared_ptr<Camera> camera)
{
	Ray ray;
	ray.SetRay(camera, mMouseX, mMouseY);
	for (int i = 0; i < mUnits.size(); ++i)
	{
		mUnits[i]->UnSelect();
		mSelectedUnit.clear();
	}

	for (int i = 0; i < mUnits.size(); ++i)
	{
		if (mUnits[i]->Intersect(ray))
		{
			mUnits[i]->Select();
			mSelectedUnit.push_back(mUnits[i]);

			return true;
		}
	}

	return false;
}

std::shared_ptr<RTSObject> GamePlayer::IsSelectEmeryUnit(std::shared_ptr<Camera> camera)
{
	Ray ray;
	ray.SetRay(camera, mMouseX, mMouseY);
	for (int i = 0; i < mEmeryUnits.size(); ++i)
	{
		shared_ptr<RTSObject> unit = mEmeryUnits[i].lock();
		if (unit != nullptr && unit->Intersect(ray))
		{
			return unit;
		}
	}

	return nullptr;
}

void GamePlayer::BuildingToPlace(int type)
{
	for (int i = 0; i < mUnits.size(); ++i)
	{
		mUnits[i]->UnSelect();
	}

	mPlayerState = PLAYER_BUILDING;
	mBuildingToPlace = std::make_shared<Building>(type);
}

void GamePlayer::NoneState()
{
	mPlayerState = PLAYER_NONE;
	mBuildingToPlace = nullptr;
}