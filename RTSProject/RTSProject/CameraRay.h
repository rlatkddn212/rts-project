#pragma once
#include "Camera.h"
#include "Mouse.h"

class CameraRay
{
public:
											CameraRay();
											CameraRay(glm::vec3 o, glm::vec3 d);
										   ~CameraRay();
	void									SetRay(std::shared_ptr<Camera> camera, int x, int y);

	glm::vec3								org;
	glm::vec3								dir;
};

