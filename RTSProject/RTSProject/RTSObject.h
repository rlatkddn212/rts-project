#pragma once

#include "Camera.h"
#include "Texture.h"

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

	void									SetFogTexture(std::shared_ptr<Texture> texture) { mFogTexture = texture; }

protected:

	std::shared_ptr<Texture>				mFogTexture;
	glm::vec3								mPos, mRot, mSca;
};

