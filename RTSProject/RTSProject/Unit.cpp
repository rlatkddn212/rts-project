#include "Precompiled.h"
#include "Unit.h"

Unit::Unit()
{

}

Unit::~Unit()
{

}

void Unit::MakeBoxObject()
{
	mBoxObject = std::make_shared<BoxObject>(mSkinnedMesh->GetMinPos(), mSkinnedMesh->GetMaxPos());
}

void Unit::Update(float deltaTime)
{
	mSkinnedMesh->Update(deltaTime);
}

void Unit::Render(std::shared_ptr<Camera> camera)
{
	mSkinnedMesh->BoneTransform();
	mSkinnedMesh->RenderModel(camera);

	mBoxObject->Render(camera);
}

void Unit::Select()
{
	mIsSelect = true;
	mBoxObject->SetVisiable(true);
}

void Unit::UnSelect()
{
	mIsSelect = false;
	mBoxObject->SetVisiable(false);
}

bool Unit::Intersect(Ray ray)
{
	glm::mat4 world = mSkinnedMesh->GetPositionMat() * mSkinnedMesh->GetRotationMat() * mSkinnedMesh->GetScaleMat();
	glm::mat4 worldInv = glm::inverse(world);
	glm::vec3 pos = worldInv * glm::vec4(ray.org, 1.0f);
	glm::vec3 dir = worldInv * glm::vec4(ray.dir, 0.0f);
	dir = glm::normalize(dir);

	if (mBoxObject->RayBoxIntersect(pos, dir))
	{
		return true;
	}

	return false;
}

glm::vec2 Unit::GetScreenPos(std::shared_ptr<Camera> camera)
{
	glm::mat4 mat = camera->GetProjectionMatrix() * camera->GetViewMatrix() * mSkinnedMesh->GetPositionMat();
	glm::vec4 pos = mat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	return glm::vec2(pos.x / pos.w, pos.y / pos.w);
}