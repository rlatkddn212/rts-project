#pragma once
#include "Precompiled.h"
#include "Mouse.h"

class Camera
{
public:
	Camera();
	~Camera();

	void Initialize();

	void Scroll(glm::vec3 vec);
	void Pitch(float f);
	void Yaw(float f);
	void Zoom(float f);
	void ChangeRadius(float f);

	void Update(float timeDelta);
	void ProcessInput(bool* state, Mouse* mouse);
	
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	float mAlpha, mBeta, mRadius, mFov;
	glm::vec3 mEye, mFocus, mRight, mLook;

	glm::mat4 matView;
	glm::mat4 matProj;
	float mTimeDelta;
};

