/**
 * OrthoCamera
 * 직교 카메라 클래스, 현재 카메라 위치를 계산해준다.
 */
#pragma once
#include "Camera.h"
class OrthoCamera : public Camera
{
public:
											OrthoCamera();
									       ~OrthoCamera();

	virtual glm::mat4						GetProjectionMatrix();
};

