#include "Precompiled.h"
#include "GameStage.h"

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
		shared_ptr<SkinnedMesh> mesh = make_shared<SkinnedMesh>();
		mesh->LoadModel("Assets/Model/magician.X");
		meshes.push_back(mesh);
	}

	for (int i = 0; i < 10; ++i)
	{
		int x = rand() % 100;
		int y = rand() % 100;
		meshes[i]->SetPosition(glm::vec3((x), terrain->GetHeight(x, y), -y));
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
		meshes[i]->Update(deltaTime);
	}
}

void GameStage::Render()
{
	WindowGroup::Render();
	axis->Render(camera);
	for (int i = 0; i < 10; ++i)
	{
		meshes[i]->BoneTransform();
		meshes[i]->RenderModel(camera);
	}
	terrain->Render(camera);

	mMouse->Render(camera);
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
		mMouse->IsDragBox(true);
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
		
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{

	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		ray.SetRay(camera, mMouseX, mMouseY);

		for (int i = 0; i < 10; ++i)
		{
			meshes[i]->UnSelect();
		}

		bool isSelect = false;
		for (int i = 0; i < 10; ++i)
		{
			if (meshes[i]->Intersect(ray))
			{
				meshes[i]->Select();
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
			for (int i = 0; i < 10; ++i)
			{
				glm::vec2 p = meshes[i]->GetScreenPos(camera);
				p.x = p.x * (mWidth / 2) + mWidth / 2;
				p.y = -p.y * (mHeight / 2) + mHeight / 2;
				
				if (mMouse->IsDragBoxPos(p))
				{
					meshes[i]->Select();
				}
			}

			isLeftPress = false;
			mMouse->IsDragBox(false);
		}
	}
}

void GameStage::MouseWheel(double yPos)
{
	camera->MouseWheel(yPos);
}