#include "Precompiled.h"
#include "Camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	mAlpha = mBeta = 0.5f;
	mRadius = 200.0f;
	mFov = glm::pi<float>() / 4.0f;

	// 눈의 위치
	mEye = glm::vec3(50.0f, 50.0f, 50.0f);
	// 눈이 가르키는 위치
	mFocus = glm::vec3(0.0f, 0.0f, 0.0f);
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
	
	if (mBeta < 0.3f)
	{
		mBeta = 0.3f;
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
}

void Camera::ProcessInput(bool* state, std::shared_ptr<Mouse> mouse)
{
	mRight.y = mLook.y = 0.0f;
	mRight = glm::normalize(mRight);
	mLook = glm::normalize(mLook);

	if (mouse->mX < 10)	Scroll(-mRight * mTimeDelta * (4.0f + mRadius * 0.2f));
	if (mouse->mX > 1024 - 10)	Scroll(mRight * mTimeDelta * (4.0f + mRadius * 0.2f));
	if (mouse->mY < 10)	Scroll(-mLook * mTimeDelta * (4.0f + mRadius * 0.2f));
	if (mouse->mY > 768 - 10)	Scroll(mLook * mTimeDelta * (4.0f + mRadius * 0.2f));

	if (mouse->mWheel > 0.0)  ChangeRadius(-10.0f);
	if (mouse->mWheel < 0.0) ChangeRadius(10.0f);

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

	float sideRadius = mRadius * cos(mBeta);
	float height = mRadius * sin(mBeta);

	mEye = glm::vec3(mFocus.x + sideRadius * cos(mAlpha), mFocus.y + height, mFocus.z + sideRadius * sin(mAlpha));
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
	float aspect = 1024.0f / 768.0f;
	matProj = glm::perspective(mFov, aspect, 1.0f, 1000.0f);
	
	return matProj;
}
