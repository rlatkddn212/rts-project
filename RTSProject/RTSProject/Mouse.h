#pragma once
#include "Precompiled.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

enum RightState
{
	RIGHT_MOUSE_PRESSED,
	RIGHT_MOUSE_RELEASE,
};

enum LeftState
{
	LEFT_MOUSE_PRESSED,
	LEFT_MOUSE_RELEASE,
};

enum MidState
{

};

enum WheelState
{
	
};

class Mouse
{
public:
											Mouse(GLFWwindow* window);
										   ~Mouse();

	void									InitMouse();
	void									SetCursorImage(int type);
	void									SetEndXY(double x, double y);
	void									SetStartXY(double x, double y);
	void									VisiableDragBox(bool t) { isVisiableDragBox = t;  }
	void									Render(std::shared_ptr<Camera> camera);
	bool									IsDragBoxPos(glm::vec2 p);
	bool									IsDragBox() { return isVisiableDragBox; }

	GLFWwindow*								mWindow;
	bool									isVisiableDragBox;
	int										mW;
	int										mH;

	std::shared_ptr<Shader>					mMeshShader;

	GLuint									mVertexBuffer;
	GLuint									mVertexArray;
	GLfloat									mPrevX;
	GLfloat									mPrevY;

	GLfloat									mX;
	GLfloat									mY;

	double									mWheel;
	double									lastWheel;

	GLFWcursor*								cursor;

	unsigned char							mPixel[6][20 * 20 * 4];

};

