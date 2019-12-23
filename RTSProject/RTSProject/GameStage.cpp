#include "Precompiled.h"
#include "GameStage.h"
#include "Magician.h"

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

	terrain = make_shared<Terrain>();
	terrain->Initialize(glm::ivec2(100, 100));
	camera = make_shared<Camera>();
	camera->Initialize(w, h);

	axis = make_shared<AxisObject>();
	isLeftPress = false;


	for (int i = 0; i < 10; ++i)
	{
		shared_ptr<Unit> mesh = make_shared<Magician>();
		mUnits.push_back(mesh);
	}

	for (int i = 0; i < 10; ++i)
	{
		int x = rand() % 100;
		int y = rand() % 100;
		mUnits[i]->SetPosition(glm::vec3((x), terrain->GetHeight(x, y), -y));
	}
}

void GameStage::Terminate()
{

}

void GameStage::Update(float deltaTime)
{
	WindowGroup::Update(deltaTime);

	camera->Update(deltaTime);
	for (int i = 0; i < 10; ++i)
	{
		mUnits[i]->Update(deltaTime);
		mUnits[i]->SetHeight(terrain);
	}
}

void GameStage::Render()
{
	WindowGroup::Render();
	axis->Render(camera);
	for (int i = 0; i < 10; ++i)
	{
		mUnits[i]->Render(camera);
	}

	terrain->Render(camera);
	mMouse->Render(camera);

	for (int i = 0; i < mBox.size(); ++i)
	{
		mBox[i]->Render(camera);
	}
}

void GameStage::PressKey(bool* keys)
{
	camera->PressKey(keys);
}

void GameStage::CursorPos(double xPos, double yPos)
{
	mMouseX = xPos;
	mMouseY = yPos;
	
	if (isLeftPress)
	{
		mMouse->SetEndXY(mMouseX, mMouseY);
		mMouse->VisiableDragBox(true);
	}
	else
	{
		camera->MouseXY(xPos, yPos);
	}
}

void GameStage::MouseButton(int button, int action)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		ray.SetRay(camera, mMouseX, mMouseY);
		glm::ivec2 pos;
		mBox.clear();
		if (terrain->Intersect(ray, pos))
		{
			for (int i = 0; i < 10; ++i)
			{
				if (mUnits[i]->isSelected())
				{
					vector<glm::ivec2> ret = terrain->GetPath(glm::ivec2(mUnits[i]->mPos.x, -mUnits[i]->mPos.z), pos);
					mUnits[i]->SetPath(ret);

					for (int i = 0; i < ret.size(); ++i)
					{
#ifdef SHOW_BOX
						shared_ptr<BoxObject> box = make_shared<BoxObject>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
						
						box->SetPosition(glm::vec3(ret[i].x, terrain->GetHeight(ret[i].x, ret[i].y), -ret[i].y));
						mBox.push_back(box);
						printf("%d %d\n", ret[i].x, ret[i].y);
#endif
					}
				}
			}
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{

	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
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
		
		if (! isSelect)
		{
			mMouse->SetStartXY(mMouseX, mMouseY);
			isLeftPress = true;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		if (isLeftPress)
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
			
			isLeftPress = false;
			mMouse->VisiableDragBox(false);
		}
	}
}

void GameStage::MouseWheel(double yPos)
{
	camera->MouseWheel(yPos);
}