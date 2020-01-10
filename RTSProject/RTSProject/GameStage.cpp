#include "Precompiled.h"
#include "GameStage.h"
#include "Math3D.h"
#include "FogOfWar.h"
#include "GamePlayer.h"

using namespace std;

GameStage::GameStage()
{
}

GameStage::~GameStage()
{
}

void GameStage::Initialize(GLFWwindow* window, int w, int h)
{
	WindowGroup::Initialize(window, w, h);

	mMoveController = std::make_shared<MoveController>();
	// 지형
	mTerrain = make_shared<Terrain>();
	mTerrain->Initialize(glm::ivec2(100, 100));

	// 카메라
	camera = make_shared<Camera>();
	camera->Initialize(w, h);

	// 좌표계
	mAxis = make_shared<AxisObject>();

	shared_ptr<Player> player = make_shared<GamePlayer>(mMouse);
	mGamePlayers = dynamic_pointer_cast<GamePlayer>(player);

	player->Initialize(mTerrain, w, h);
	mPlayers.push_back(player);

	mFogOfWar = make_shared<FogOfWar>(w, h);

	mMiniMap = make_shared<MiniMap>(w, h, w * 3.0f / 10.0f, h * 3.0f / 10.0f);
	mMiniMap->SetMapTexture(mTerrain->GetMapTexture());
}

void GameStage::Terminate()
{

}

void GameStage::Update(float deltaTime)
{
	WindowGroup::Update(deltaTime);

	camera->Update(deltaTime);

	for (auto player : mPlayers)
	{
		player->Update(deltaTime);
	}

	mFogOfWar->Update(deltaTime, mPlayers[0]->GetUnit());
	mMiniMap->SetFogTexture(mFogOfWar->GetFogTexture());
	mMiniMap->Update(deltaTime, mPlayers[0]->GetUnit(), camera);
}

void GameStage::Render()
{
	WindowGroup::Render();

	mAxis->Render(camera);
	shared_ptr<Texture> fog = mFogOfWar->GetFogTexture();

	for (auto player : mPlayers)
	{
		player->SetFogTexture(fog);
		player->Render(camera);
	}

	mTerrain->SetFogTexture(fog);
	mTerrain->Render(camera);
	mMouse->Render(camera);

	mMiniMap->Render(camera);
}

void GameStage::PressKey(bool* keys)
{
	camera->PressKey(keys);
	mGamePlayers->PressKey(keys);
}

void GameStage::CursorPos(double xPos, double yPos)
{
	mMouseX = xPos;
	mMouseY = yPos;

	if (mMouse->IsDragBox())
	{
		mMouse->SetEndXY(mMouseX, mMouseY);
	}
	else
	{
		mGamePlayers->CursorPos(camera, xPos, yPos);
		camera->MouseXY(xPos, yPos);
	}
}

void GameStage::MouseButton(int button, int action)
{
	// 오른쪽 마우스 클릭시 유닛 이동
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{

	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{

	}
	// 왼쪽 마우스 클릭시
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		// 미니맵 클릭시
		if (mMiniMap->IsXYInMiniMap(mMouseX, mMouseY))
		{
			// 미니맵을 클릭
			
			// 좌표계산
			glm::vec2 pos = mMiniMap->GetTerrainPos(mMouseX, mMouseY);
			assert(0.0f < pos.x || pos.x <= 100.0f);
			assert(0.0f < pos.y || pos.y <= 100.0f);
			// 카메라를 계산된 좌표를 보도록 위치 수정
			camera->SetPos(pos);
			
			return;
		}
		else
		{
			mMouse->SetStartXY(mMouseX, mMouseY);
			mMouse->SetEndXY(mMouseX, mMouseY);
			mMouse->VisiableDragBox(true);
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		if (mMouse->IsDragBox())
		{
			mGamePlayers->SelectUnitInRect(camera);
		}

		mMouse->VisiableDragBox(false);
	}

	// UI가 클릭된 경우 게임 화면에 마우스 클릭 무시
	mGamePlayers->MouseButton(camera, button, action);
}

void GameStage::MouseWheel(double yPos)
{
	camera->MouseWheel(yPos);
}