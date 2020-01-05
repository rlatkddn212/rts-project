#include "Precompiled.h"
#include "GamePlayer.h"

using namespace std;

GamePlayer::GamePlayer()
{
}

GamePlayer::~GamePlayer()
{
}

void GamePlayer::Initialize(shared_ptr<Terrain> terrain, int w, int h)
{
	Player::Initialize(terrain, w, h);
	mWidth = w;
	mHeight = h;

	playerState = PLAYER_NONE;
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

void GamePlayer::PressKey(bool * keys)
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
	else if (keys[GLFW_KEY_ESCAPE] == true)
	{
		NoneState();
	}
}

void GamePlayer::CursorPos(shared_ptr<Mouse> mouse, std::shared_ptr<Camera> camera, double xPos, double yPos)
{
	mMouseX = xPos;
	mMouseY = yPos;

	if (playerState == PLAYER_BUILDING)
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

void GamePlayer::MouseButton(shared_ptr<Mouse> mouse, std::shared_ptr<Camera> camera, int button, int action)
{
	// 오른쪽 마우스 클릭시 유닛 이동
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		Ray ray;
		ray.SetRay(camera, mMouseX, mMouseY);
		glm::ivec2 pos;
		if (mTerrain->Intersect(ray, pos))
		{
			if (mTerrain->IsMovableTile(pos))
			{
				for (int i = 0; i < 10; ++i)
				{
					if (mUnits[i]->isSelected())
					{
						mUnits[i]->SetMove(mTerrain, pos);
					}
				}
			}
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{

	}
	// 왼쪽 마우스 클릭시
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		// 기본 상태일 경우 유닛 선택 확인
		if (playerState == PLAYER_NONE)
		{
			if (IsSelectUnit(mouse, camera))
			{
				mouse->VisiableDragBox(true);
			}
		}
		// 건물 짓기 준비 상태일 경우
		else if (playerState == PLAYER_BUILDING)
		{
			CreateBuilding(mouse, camera);
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{

	}
}

void GamePlayer::MouseWheel(std::shared_ptr<Mouse> mouse, std::shared_ptr<Camera> camera, double yPos)
{
}

bool GamePlayer::CreateBuilding(std::shared_ptr<Mouse> mouse, std::shared_ptr<Camera> camera)
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

void GamePlayer::SelectUnit(std::shared_ptr<Mouse> mouse, std::shared_ptr<Camera> camera)
{
	for (int i = 0; i < 10; ++i)
	{
		glm::vec2 p = mUnits[i]->GetScreenPos(camera);
		p.x = p.x * (mWidth / 2) + mWidth / 2;
		p.y = -p.y * (mHeight / 2) + mHeight / 2;

		if (mouse->IsDragBoxPos(p))
		{
			mUnits[i]->Select();
		}
	}
}

bool GamePlayer::IsSelectUnit(shared_ptr<Mouse> mouse, std::shared_ptr<Camera> camera)
{
	Ray ray;
	ray.SetRay(camera, mMouseX, mMouseY);
	for (int i = 0; i < 10; ++i)
	{
		mUnits[i]->UnSelect();
	}

	bool isSelect = false;
	for (int i = 0; i < 10; ++i)
	{
		if (mUnits[i]->Intersect(ray))
		{
			mUnits[i]->Select();
			isSelect = true;

			return true;
		}
	}

	return false;
}

void GamePlayer::BuildingToPlace(int type)
{
	for (int i = 0; i < 10; ++i)
	{
		mUnits[i]->UnSelect();
	}

	playerState = PLAYER_BUILDING;
	mBuildingToPlace = std::make_shared<Building>(type);
}

void GamePlayer::NoneState()
{
	playerState = PLAYER_NONE;
	mBuildingToPlace = nullptr;
}