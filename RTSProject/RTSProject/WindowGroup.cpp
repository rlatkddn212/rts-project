#include "Precompiled.h"
#include "WindowGroup.h"
#include "Font.h"
#include "Shader.h"

WindowGroup::WindowGroup()
{
}

WindowGroup::~WindowGroup()
{
}

void WindowGroup::Initialize(GLFWwindow* window, int w, int h)
{
	win = window;
	mWidth = w;
	mHeight = h;

	mMouseX = w / 2;
	mMouseY = h / 2;

	mText = std::make_shared<TextUI>();
	const char* str = "wasd";
	mText->SetText(str);

	mMouse = std::make_shared<Mouse>(window);
}

void WindowGroup::Terminate()
{
}

void WindowGroup::Update(float deltaTime)
{
}

void WindowGroup::Render()
{
	mText->Render(mCamera);
}

void WindowGroup::PressKey(bool* keys)
{
}

void WindowGroup::CursorPos(double xPos, double yPos)
{
}

void WindowGroup::MouseButton(int button, int action)
{
}

void WindowGroup::MouseWheel(double yPos)
{
}
