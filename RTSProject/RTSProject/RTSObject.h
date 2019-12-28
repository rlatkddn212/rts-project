#pragma once

#include "Camera.h"

class RTSObject
{
public:
											RTSObject();
	virtual								   ~RTSObject();

	virtual	void							Update(float deltaTime) = 0;
	virtual void							Render(std::shared_ptr<Camera> camera) = 0;

	void									SetPosition(glm::vec3 p) { mPos = p; }
	void									SetRotation(glm::vec3 r) { mRot = r; }
	void									SetScale(glm::vec3 s) { mSca = s; }

protected:

	glm::vec3								mPos, mRot, mSca;
};

