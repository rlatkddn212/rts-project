#include "Precompiled.h"
#include "OrthoCamera.h"


OrthoCamera::OrthoCamera()
{
}


OrthoCamera::~OrthoCamera()
{
}

glm::mat4 OrthoCamera::GetProjectionMatrix()
{
	glm::mat4 matProj;
	//matProj = glm::ortho(-50.0f, 50.f, -50.0f, 50.0f, -1000.0f, 1000.0f);

	return glm::mat4(1.0f);
}
