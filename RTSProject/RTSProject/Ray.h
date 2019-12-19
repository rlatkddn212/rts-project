#pragma once
#include "Precompiled.h"
#include "Camera.h"
#include "Mouse.h"

class Ray
{
public:
	Ray(std::shared_ptr<Camera> camera, std::shared_ptr<Mouse> mouse);
	Ray(glm::vec3 o, glm::vec3 d);
	~Ray();

	glm::vec3 org;
	glm::vec3 dir;
};

