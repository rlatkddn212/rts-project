#include "Precompiled.h"
#include "GameStage.h"
#include "Math3D.h"
#include "FogOfWar.h"
#include "GamePlayer.h"
#include "AiPlayer.h"

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
	mCamera = make_shared<Camera>();
	mCamera->Initialize(w, h);

	// 좌표계
	mAxis = make_shared<AxisObject>();

	shared_ptr<Player> player = make_shared<GamePlayer>(mMouse);
	mGamePlayers = dynamic_pointer_cast<GamePlayer>(player);

	player->Initialize(mTerrain, w, h);
	mPlayers.push_back(player);

	shared_ptr<Player> aiPlayer = make_shared<AiPlayer>();
	aiPlayer->Initialize(mTerrain, w, h);
	mPlayers.push_back(aiPlayer);

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

	mCamera->Update(deltaTime);

	for (auto player : mPlayers)
	{
		vector<shared_ptr<RTSObject> > obj;
		for (int i = 0; i < mPlayers.size(); ++i)
		{
			if (player != mPlayers[i])
			{
				vector<shared_ptr<Unit> > unitObj = mPlayers[i]->GetUnit();
				obj.insert(obj.begin(), unitObj.begin(), unitObj.end());
			}
		}
		player->SetEnemy(obj);
		player->Update(deltaTime);
	}

	mFogOfWar->Update(deltaTime, mPlayers[0]->GetUnit());
	mMiniMap->SetFogTexture(mFogOfWar->GetFogTexture());
	mMiniMap->Update(deltaTime, mPlayers[0]->GetUnit(), mCamera);
}

void GameStage::Render()
{
	mAxis->Render(mCamera);
	shared_ptr<Texture> fog = mFogOfWar->GetFogTexture();

	mTerrain->SetFogTexture(fog);
	mTerrain->Render(mCamera);
	for (auto player : mPlayers)
	{
		player->SetFogTexture(fog);
		player->Render(mCamera);
	}

	mMiniMap->Render(mCamera);
	mMouse->Render(mCamera);
	WindowGroup::Render();

}

void GameStage::PressKey(bool* keys)
{
	mCamera->PressKey(keys);
	mGamePlayers->PressKey(keys);
}

void GameStage::CursorPos(double xPos, double yPos)
{
	if (xPos < 0)
	{
		xPos = 0;
	}
	if (mWidth < xPos)
	{
		xPos = mWidth;
	}
	if (yPos < 0)
	{
		yPos = 0;
	}
	if (mHeight < yPos)
	{
		yPos = mHeight;
	}
	
	glfwSetCursorPos(win, xPos, yPos);

	mMouseX = xPos;
	mMouseY = yPos;
	mMouse->SetCursorPos(glm::vec2(mMouseX, mMouseY));

	if (mMouse->IsDragBox())
	{
		mMouse->SetEndXY(mMouseX, mMouseY);
	}
	else
	{
		mGamePlayers->CursorPos(mCamera, xPos, yPos);
		mCamera->MouseXY(xPos, yPos);
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
			// 좌표계산
			glm::vec2 pos = mMiniMap->GetTerrainPos(mMouseX, mMouseY);
			assert(0.0f < pos.x || pos.x <= 100.0f);
			assert(0.0f < pos.y || pos.y <= 100.0f);
			// 카메라를 계산된 좌표를 보도록 위치 수정
			mCamera->SetPos(pos);
			
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
			mGamePlayers->SelectUnitInRect(mCamera);
		}

		mMouse->VisiableDragBox(false);
	}

	// UI가 클릭된 경우 게임 화면에 마우스 클릭 무시
	mGamePlayers->MouseButton(mCamera, button, action);
}

void GameStage::MouseWheel(double yPos)
{
	mCamera->MouseWheel(yPos);
}