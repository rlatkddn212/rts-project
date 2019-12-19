#pragma once
#include "Precompiled.h"
#include "Texture.h"

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

	void InitMouse(GLFWwindow* window);
	void Update(GLfloat x, GLfloat y, double wheel);
	
	int mW;
	int mH;
	GLfloat mX;
	GLfloat mY;

	double mWheel;
	double lastWheel;

	GLFWcursor* cursor;
};

