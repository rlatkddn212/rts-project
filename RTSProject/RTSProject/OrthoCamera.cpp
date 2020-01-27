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
	return glm::ortho(-50.0f, 50.f, -50.0f, 50.0f, -100.0f, 100.0f);
}
