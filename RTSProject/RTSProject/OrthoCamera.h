/**
 * OrthoCamera
 * ���� ī�޶� Ŭ����, ���� ī�޶� ��ġ�� ������ش�.
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

