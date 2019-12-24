#include "Precompiled.h"
#include "Unit.h"

using namespace std;

Unit::Unit()
{
	speed = 3.0f;
}

Unit::~Unit()
{

}

void Unit::MakeBoxObject()
{
	mBoxObject = std::make_shared<BoxObject>(mSkinnedMesh->GetMinPos(), mSkinnedMesh->GetMaxPos());
	mBoxObject->SetScale(glm::vec3(0.3f, 0.3, 0.3f));
	SetScale(glm::vec3(0.3f, 0.3, 0.3f));
}

glm::vec3 Unit::GetDirection(glm::vec2 p1, glm::vec2 p2)
{
	float dx = p2.x - p1.x, dy = p2.y - p1.y;

	if (dx < 0 && dy > 0)	return glm::vec3(glm::pi<float>() / 4, 0.0f, 0.0f);
	if (dx == 0 && dy > 0)	return glm::vec3(0.0f, 0.0f, 0.0f);
	if (dx > 0 && dy > 0)	return glm::vec3(-glm::pi<float>() / 4, 0.0f, 0.0f);
	if (dx > 0 && dy == 0)	return glm::vec3(-glm::pi<float>() / 2, 0.0f, 0.0f);
	if (dx > 0 && dy < 0)	return glm::vec3((-glm::pi<float>() / 4) * 3, 0.0f, 0.0f);
	if (dx == 0 && dy < 0)	return glm::vec3(glm::pi<float>(), 0.0f, 0.0f);
	if (dx < 0 && dy < 0)	return glm::vec3((glm::pi<float>() / 4) * 3, 0.0f, 0.0f);
	if (dx < 0 && dy == 0)	return glm::vec3(glm::pi<float>() / 2, 0.0f, 0.0f);

	return mRot;
}

void Unit::Update(float deltaTime)
{
	mSkinnedMesh->Update(deltaTime);
	if (!mPath.empty())
	{
		SetAnimation(2);
		float len = deltaTime * speed;
		glm::vec2 prev = glm::vec2(mPos.x, -mPos.z);
		for (int i = mPosIndex; i < mPath.size(); ++i)
		{
			float d = glm::distance(prev, glm::vec2(mPath[i]));
			len -= d;
			if (len < 0.0f)
			{
				glm::vec2 pos = prev + (1 - (-len / d)) *  (glm::vec2(mPath[i]) - prev);
				
				mRot = GetDirection(glm::vec2(mPath[i]), prev);
				SetRotation(mRot);
				mPos.x = pos.x;
				mPos.z = -pos.y;

				SetPosition(mPos);
				break;
			}
			
			++mPosIndex;
			prev = mPath[i];
		}

		if (len > 0)
		{
			SetAnimation(1);
			mPos.x = mPath[mPath.size() - 1].x;
			mPos.z = -mPath[mPath.size() - 1].y;

			SetPosition(mPos);
		}
	}
	else
	{
		SetAnimation(1);
	}
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

void Unit::SetPosOnTerrain(shared_ptr<Terrain> terrain, float x, float y)
{
	terrain->SetUnitOnTile(x, y);
	SetPosition(glm::vec3(x, 0.0f, -y));
	SetHeight(terrain);
}
