#include "Precompiled.h"
#include "Ray.h"

Ray::Ray()
{
}

Ray::Ray(glm::vec3 o, glm::vec3 d)
{
	org = o;
	dir = d;
}

Ray::~Ray()
{
}

void Ray::SetRay(std::shared_ptr<Camera> camera, int x, int y)
{
	glm::mat4 proj = camera->GetProjectionMatrix();
	glm::mat4 view = camera->GetViewMatrix();

	float p0 = proj[0][0];
	float p1 = proj[1][1];

	float width = camera->mWidth;
	float height = camera->mHeight;
	float angleX = (((2.0f * x) / width) - 1.0f);
	float angleY = (1.0f - (2.0f * y) / height);

	org = glm::vec3(0.0f, 0.0f, 0.0f);
	dir = glm::inverse(proj) * glm::vec4(angleX, angleY, 0.0f, 1.0f);

	glm::mat4 viewInv = glm::inverse(view);

	org = viewInv * glm::vec4(org, 1.0f);
	// 방향이므로 이동 성분은 계산되면 안된다.
	dir = viewInv * glm::vec4(dir, 0.0f);
	dir = glm::normalize(dir);
}