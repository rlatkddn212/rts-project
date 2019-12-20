#pragma once
#include "Precompiled.h"
#include "Camera.h"
#include "Mouse.h"

class Ray
{
public:
											Ray();
											Ray(glm::vec3 o, glm::vec3 d);
										   ~Ray();
	void									SetRay(std::shared_ptr<Camera> camera, int x, int y);

	glm::vec3								org;
	glm::vec3								dir;
};

