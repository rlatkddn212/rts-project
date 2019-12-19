#pragma once
#include "Precompiled.h"
#include "Camera.h"
#include "Mouse.h"

class Ray
{
public:
	Ray(Camera * camera, Mouse* mouse);
	Ray(glm::vec3 o, glm::vec3 d);
	~Ray();

	glm::vec3 org;
	glm::vec3 dir;
};

