#include "Precompiled.h"
#include "GameStage.h"
#include "Magician.h"
#include "Math3D.h"

using namespace std;

GameStage::GameStage()
{
}

GameStage::~GameStage()
{
}

void GameStage::Initialize(GLFWwindow* window, int w, int h)
{
	playerState = PLAYER_NONE;

	WindowGroup::Initialize(window, w, h);

	mMoveController = std::make_shared<MoveController>();
	// ����
	mTerrain = make_shared<Terrain>();
	mTerrain->Initialize(glm::ivec2(100, 100));
	
	// ī�޶�
	camera = make_shared<Camera>();
	camera->Initialize(w, h);

	// ��ǥ��
	mAxis = make_shared<AxisObject>();
	mIsLeftPress = false;

	// ���� ��ġ
	for (int i = 0; i < 10; ++i)
	{
		shared_ptr<Unit> mesh = make_shared<Magician>();
		mUnits.push_back(mesh);
	}

	for (int i = 0; i < 10; ++i)
	{
		int x = rand() % 100;
		int y = rand() % 100;
		mUnits[i]->SetPosOnTerrain(mTerrain, glm::ivec2(x, y));
	}

	// �ǹ�
	mBuildingToPlace = nullptr;
}

void GameStage::Terminate()
{

}

void GameStage::Update(float deltaTime)
{
	WindowGroup::Update(deltaTime);

	mTerrain->InitUnitTile();
	camera->Update(deltaTime);
	for (int i = 0; i < 10; ++i)
	{
		mUnits[i]->Update(deltaTime);
		glm::vec3 unitPos = mUnits[i]->GetPosition();
		mUnits[i]->SetPosOnTerrain(mTerrain, glm::vec2(unitPos.x, -unitPos.z));
	}

	mMoveController->SetTerrain(mTerrain);
	mMoveController->SetUnit(mUnits);

	mMoveController->Update(deltaTime);
}

void GameStage::Render()
{
	WindowGroup::Render();
	mAxis->Render(camera);
	for (int i = 0; i < 10; ++i)
	{
		mUnits[i]->Render(camera);
	}

	mTerrain->Render(camera);
	mMouse->Render(camera);

	for (int i = 0; i < mBox.size(); ++i)
	{
		mBox[i]->Render(camera);
	}

	if (mBuildingToPlace)
	{
		mBuildingToPlace->RenderModel(camera);
	}

	for (int i = 0; i < mBuildings.size(); ++i)
	{
		mBuildings[i]->RenderModel(camera);
	}
}

void GameStage::PressKey(bool* keys)
{
	camera->PressKey(keys);


	// �ǹ� ��ġ ����
	if (keys[GLFW_KEY_1] == true)
	{
		for (int i = 0; i < 10; ++i)
		{
			mUnits[i]->UnSelect();
		}

		playerState = PLAYER_BUILDING;
		mBuildingToPlace = std::make_shared<Building>(0);
	}
	else if (keys[GLFW_KEY_2] == true)
	{
		for (int i = 0; i < 10; ++i)
		{
			mUnits[i]->UnSelect();
		}

		playerState = PLAYER_BUILDING;
		mBuildingToPlace = std::make_shared<Building>(1);
	}
	else if (keys[GLFW_KEY_3] == true)
	{
		for (int i = 0; i < 10; ++i)
		{
			mUnits[i]->UnSelect();
		}

		playerState = PLAYER_BUILDING;
		mBuildingToPlace = std::make_shared<Building>(2);
	}
	else if (keys[GLFW_KEY_ESCAPE] == true)
	{
		playerState = PLAYER_NONE;
		mBuildingToPlace = nullptr;
	}
}

void GameStage::CursorPos(double xPos, double yPos)
{
	mMouseX = xPos;
	mMouseY = yPos;
	
	if (playerState == PLAYER_BUILDING)
	{
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
	else
	{
		if (mIsLeftPress)
		{
			mMouse->SetEndXY(mMouseX, mMouseY);
			mMouse->VisiableDragBox(true);
		}
		else
		{
			camera->MouseXY(xPos, yPos);
		}
	}
}

void GameStage::MouseButton(int button, int action)
{
	// ������ ���콺 Ŭ���� ���� �̵�
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		ray.SetRay(camera, mMouseX, mMouseY);
		glm::ivec2 pos;
		mBox.clear();
		if (mTerrain->Intersect(ray, pos))
		{
			for (int i = 0; i < 10; ++i)
			{
				if (mUnits[i]->isSelected())
				{
					mUnits[i]->SetMove(mTerrain, pos);

#ifdef SHOW_BOX
					for (int i = 0; i < ret.size(); ++i)
					{
						shared_ptr<BoxObject> box = make_shared<BoxObject>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
						
						box->SetPosition(glm::vec3(ret[i].x, mTerrain->GetHeight(ret[i].x, -ret[i].y), -ret[i].y));
						mBox.push_back(box);
						printf("%d %d\n", ret[i].x, ret[i].y);
					}
#endif
				}
			}
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{

	}
	// ���� ���콺 Ŭ����
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		// �⺻ ������ ��� ���� ���� Ȯ��
		if (playerState == PLAYER_NONE)
		{
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

					break;
				}
			}
			if (!isSelect)
			{
				mMouse->SetStartXY(mMouseX, mMouseY);
				mIsLeftPress = true;
			}
		}
		// �ǹ� ���� �غ� ������ ���
		else if (playerState == PLAYER_BUILDING)
		{
			ray.SetRay(camera, mMouseX, mMouseY);
			glm::ivec2 pos;
			mBox.clear();
			if (mTerrain->Intersect(ray, pos))
			{
				// ���� �Ǽ� ������ �����̶�� �ǹ� �߰�
				if (mBuildingToPlace->isPossibleBuild(mTerrain, pos.x, pos.y))
				{
					shared_ptr<Building> building = make_shared<Building>(mBuildingToPlace->GetType());
					building->BuildOnTerrain(mTerrain, pos.x, pos.y);
					mBuildings.push_back(building);
				}
			}
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		if (mIsLeftPress)
		{
			if (mMouse->IsDragBox())
			{
				for (int i = 0; i < 10; ++i)
				{
					glm::vec2 p = mUnits[i]->GetScreenPos(camera);
					p.x = p.x * (mWidth / 2) + mWidth / 2;
					p.y = -p.y * (mHeight / 2) + mHeight / 2;

					if (mMouse->IsDragBoxPos(p))
					{
						mUnits[i]->Select();
					}
				}
			}
			
			mIsLeftPress = false;
			mMouse->VisiableDragBox(false);
		}
	}
}

void GameStage::MouseWheel(double yPos)
{
	camera->MouseWheel(yPos);
}