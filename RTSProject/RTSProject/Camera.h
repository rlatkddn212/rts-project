#pragma once
#include "Precompiled.h"

class Camera
{
public:
											Camera();
										   ~Camera();

	void									Initialize(int w, int h);

	void									Scroll(glm::vec3 vec);
	void									Pitch(float f);
	void									Yaw(float f);
	void									Zoom(float f);
	void									ChangeRadius(float f);

	void									Update(float timeDelta);
	void									PressKey(bool* keys);
	void									MouseXY(int x, int y);
	void									MouseWheel(int y);

	glm::mat4								GetViewMatrix();
	glm::mat4								GetProjectionMatrix();

	void									SetPos(glm::vec2 pos);

	GLfloat									mX;
	GLfloat									mY;

	double									mWheel;
	bool*									state;


	float									mAlpha, mBeta, mRadius, mFov;
	glm::vec3								mEye, mFocus, mRight, mLook;

	glm::mat4								mMatView;
	glm::mat4								mMatProj;
	float									mTimeDelta;

	int										mWidth;
	int										mHeight;
};

