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
	return glm::ortho(-100.0f, 100.f, -100.0f, 100.0f, 0.0f, 200.0f);
}
