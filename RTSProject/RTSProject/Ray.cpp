#include "Precompiled.h"
#include "Ray.h"

Ray::Ray(std::shared_ptr<Camera> camera, std::shared_ptr<Mouse> mouse)
{
	glm::mat4 proj = camera->GetProjectionMatrix();
	glm::mat4 view = camera->GetViewMatrix();

	float p0 = proj[0][0];
	float p1 = proj[1][1];
	
	float width = 1024;
	float height = 768;
	float angleX2 = (((2.0f * mouse->mX) / width) - 1.0f) / p0;
	float angleY2 = (1.0f - (2.0f * mouse->mY) / height) / p1;

	float angleX = (((2.0f * mouse->mX) / width) - 1.0f);
	float angleY = (1.0f - (2.0f * mouse->mY) / height);

	org = glm::vec3(0.0f, 0.0f, 0.0f);
	dir = glm::inverse(proj) * glm::vec4(angleX, angleY, 0.0f, 1.0f);

	glm::mat4 viewInv = glm::inverse(view);
	
	org = viewInv * glm::vec4(org, 1.0f);
	// 방향이므로 이동 성분은 계산되면 안된다.
	dir = viewInv * glm::vec4(dir, 0.0f);
	dir = glm::normalize(dir);
}

Ray::Ray(glm::vec3 o, glm::vec3 d)
{
	org = o;
	dir = d;
}

Ray::~Ray()
{
} 