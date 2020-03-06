/**
 * Camera
 * 일반 카메라 클래스, 현재 카메라 위치를 계산해준다.
 */

#pragma once

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
	virtual glm::mat4						GetProjectionMatrix();

	void									SetPos(glm::vec2 pos);

	void									SetCameraPos(glm::vec3 pos) { mEye = pos; }
	void									SetFocus(glm::vec3 focus) { mFocus = focus; }
	glm::vec3								GetCameraPos() { return mEye; }

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

