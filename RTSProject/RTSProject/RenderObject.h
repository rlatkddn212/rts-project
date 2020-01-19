#pragma once
#include "Camera.h"
#include "Camera.h"
#include "Texture.h"

class RenderObject
{
public:
	virtual void							Render(std::shared_ptr<Camera> camera);
	virtual void							AddRender(std::shared_ptr<Camera> camera);
	virtual void							MakeModel();
	virtual void							UpdateModel();

	std::shared_ptr<Camera>					mCamera;
};