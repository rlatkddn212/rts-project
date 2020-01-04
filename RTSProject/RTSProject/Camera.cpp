#include "Precompiled.h"
#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Initialize(int w, int h)
{
	state = NULL;
	mWidth = w;
	mHeight = h;

	mAlpha = mBeta = 0.5f;
	mRadius = 200.0f;
	mFov = glm::pi<float>() / 4.0f;

	// 눈의 위치
	mEye = glm::vec3(50.0f, 50.0f, 50.0f);
	// 눈이 가르키는 위치
	mFocus = glm::vec3(0.0f, 0.0f, 0.0f);

	mX = w/2;
	mY = h/2;
	mWheel = 0;
}

void Camera::Scroll(glm::vec3 vec)
{
	glm::vec3 newFocus = mFocus + vec;

	if (newFocus.x > -500.0f && newFocus.x < 500.0f &&
		newFocus.z > -500.0f && newFocus.z < 500.0f)
	{
		mFocus = newFocus;
	}
}

void Camera::Pitch(float f)
{
	mBeta += f;

	if (mBeta > glm::pi<float>() / 2.0f)
	{
		mBeta = glm::pi<float>() / 2.0f - 0.01f;
	}
	
	if (mBeta < 0.5f)
	{
		mBeta = 0.5f;
	}
}

void Camera::Yaw(float f)
{
	mAlpha += f;
	if (mAlpha > glm::pi<float>() * 2.0f)mAlpha -= glm::pi<float>() * 2.0f;
	if (mAlpha < -glm::pi<float>() * 2.0f)mAlpha += glm::pi<float>() * 2.0f;
}

void Camera::Zoom(float f)
{
	mFov += f;

	if (mFov < 0.1f)mFov = 0.1f;
	if (mFov > glm::pi<float>() / 2.0f)mFov = glm::pi<float>() / 2.0f;
}

void Camera::ChangeRadius(float f)
{
	mRadius += f;

	if (mRadius < 5.0f)
	{
		mRadius = 5.0f;
	}

	if (mRadius > 10000.0f)
	{
		mRadius = 10000.0f;
	}
}

void Camera::Update(float timeDelta)
{
	mTimeDelta = timeDelta;

	mRight.y = mLook.y = 0.0f;
	mRight = glm::normalize(mRight);
	mLook = glm::normalize(mLook);

	if (mX < 10)			Scroll(-mRight * mTimeDelta * (4.0f + mRadius * 0.2f));
	if (mX > mWidth - 10)	Scroll(mRight * mTimeDelta * (4.0f + mRadius * 0.2f));
	if (mY < 10)			Scroll(-mLook * mTimeDelta * (4.0f + mRadius * 0.2f));
	if (mY > mHeight - 10)	Scroll(mLook * mTimeDelta * (4.0f + mRadius * 0.2f));

	if (mWheel > 0.0)  ChangeRadius(-10.0f);
	if (mWheel < 0.0) ChangeRadius(10.0f);

	if (state != NULL)
	{
		if (state[GLFW_KEY_LEFT])
		{
			Scroll(-mRight * mTimeDelta * 20.0f);
		}
		if (state[GLFW_KEY_RIGHT])
		{
			Scroll(mRight * mTimeDelta * 20.0f);
		}
		if (state[GLFW_KEY_DOWN])
		{
			Scroll(mLook * mTimeDelta * 20.0f);
		}
		if (state[GLFW_KEY_UP])
		{
			Scroll(-mLook * mTimeDelta  * 20.0f);
		}

		if (state[GLFW_KEY_A])
		{
			Yaw(-mTimeDelta);
		}
		if (state[GLFW_KEY_D])
		{
			Yaw(mTimeDelta);
		}
		if (state[GLFW_KEY_W])
		{
			Pitch(mTimeDelta);
		}
		if (state[GLFW_KEY_S])
		{
			Pitch(-mTimeDelta);
		}

		if (state[GLFW_KEY_LEFT_SHIFT])
		{
			if (state[GLFW_KEY_KP_ADD])Zoom(-mTimeDelta);
			if (state[GLFW_KEY_KP_SUBTRACT])Zoom(mTimeDelta);
		}
		else
		{
			if (state[GLFW_KEY_KP_ADD])ChangeRadius(-mTimeDelta * 100.0f);
			if (state[GLFW_KEY_KP_SUBTRACT])ChangeRadius(mTimeDelta * 100.0f);
		}
	}

	float sideRadius = mRadius * cos(mBeta);
	float height = mRadius * sin(mBeta);

	mEye = glm::vec3(mFocus.x + sideRadius * cos(mAlpha), mFocus.y + height, mFocus.z + sideRadius * sin(mAlpha));
	mWheel = 0;
}

void Camera::PressKey(bool* keys)
{
	state = keys;
}

void Camera::MouseXY(int x, int y)
{
	mX = x;
	mY = y;
}

void Camera::MouseWheel(int y)
{
	mWheel = y;
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 matView = glm::lookAt(mEye, mFocus, glm::vec3(0.0f, 1.0f, 0.0f));

	mRight.x = matView[0][0];
	mRight.y = matView[1][0];
	mRight.z = matView[2][0];
	mRight= glm::normalize(mRight);
	
	mLook.x = matView[0][2];
	mLook.y = matView[1][2];
	mLook.z = matView[2][2];
	mLook = glm::normalize(mLook);

	return  matView;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	glm::mat4 matProj;
	float aspect = (float)mWidth / mHeight;
	matProj = glm::perspective(mFov, aspect, 1.0f, 1000.0f);
	
	return matProj;
}

void Camera::SetPos(glm::vec2 pos)
{
	// 현재 카메라가 보고 있는 위치와 이동할 위치의 차
	glm::vec2 d = glm::vec2(mFocus.x - pos.x, mFocus.z - pos.y);
	// mFocus가 보는 위치를 고려하자.
	mFocus = glm::vec3(pos.x, mFocus.y, pos.y);
	mEye = glm::vec3(mEye.x - d.x, mEye.y, mEye.z - d.y);
}
