#pragma once
#include "Precompiled.h"
#include "Shader.h"
#include "Font.h"
#include "VertexArray.h"
#include "TextUI.h"
#include "Camera.h"
#include "Ray.h"
#include "Mouse.h"

class WindowGroup
{
public:
											WindowGroup();
	virtual								   ~WindowGroup();

	virtual void							Initialize(GLFWwindow* window, int w, int h);
	virtual void							Terminate();
	virtual void							Update(float deltaTime);
	virtual void							Render();

	virtual void							PressKey(bool* keys);
	virtual void							CursorPos(double xPos, double yPos);
	virtual void							MouseButton(int button, int action);
	virtual void							MouseWheel(double yPos);

	GLFWwindow*								win;
	GLuint									VertexArrayID;
	GLuint									vertexbuffer;
	GLuint									uvBuffer;
	
	int										mWidth;
	int										mHeight;

	std::shared_ptr<Camera>					camera;
	std::shared_ptr<TextUI>					mText;
	std::shared_ptr<Mouse>					mMouse;

	Ray										ray;
	int										mMouseX;
	int										mMouseY;
};
