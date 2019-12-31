#include "Precompiled.h"
#include "Unit.h"
#include "Math3D.h"

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
	/*
	if (!mPath.empty())
	{
		SetAnimation(2);
		float len = deltaTime * speed;
		glm::vec2 prev = glm::vec2(mPos.x, -mPos.z);

		if (mPathIdx == 0)
		{
			mPathIdx = 1;
		}

		for (int i = mPathIdx; i < mPath.size(); ++i)
		{
			float d = glm::distance(prev, glm::vec2(mPath[i]));
			len -= d;
			
			if (len < 0.0f)
			{
				glm::vec2 pos = prev + (1 - (-len / d)) * (glm::vec2(mPath[i]) - prev);
				
				SetRotation(GetDirection(glm::vec2(mPath[i]), prev));
				mPos.x = pos.x;
				mPos.z = -pos.y;

				SetPosition(mPos);

				break;
			}
			
			++mPathIdx;
			prev = mPath[i];
		}

		// 도착한 경우
		if (len > 0)
		{
			SetAnimation(1);
			mPos.x = mPath[mPath.size() - 1].x;
			mPos.z = -mPath[mPath.size() - 1].y;
			mPath.clear();
			SetPosition(mPos);
		}
	}
	else
	{
		SetAnimation(1);
	}
	*/
}

void Unit::Render(std::shared_ptr<Camera> camera)
{
	mSkinnedMesh->BoneTransform();

	glActiveTexture(GL_TEXTURE1);
	mFogTexture->SetActive();
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

void Unit::SetPath(const std::vector<glm::ivec2>& path)
{
	mPathIdx = 0;
	mPath.clear();
	mPath = path;
}




void Unit::SetMove(std::shared_ptr<Terrain> terrain, glm::ivec2 movePos)
{
	vector<glm::ivec2> ret = terrain->GetPath(RoundPosition(glm::vec2(mPos.x, -mPos.z)), movePos);
	SetPath(ret);

	if (!ret.empty())
	{
		mMovePos = movePos;
	}
}

void Unit::SetPosOnTerrain(shared_ptr<Terrain> terrain, glm::vec2 p)
{
	// 이미 타일에 다른 오브젝트가 있다면
	/*
	if (terrain->IsUnitOnTile(RoundPosition(p)) || terrain->IsObjectOnTile(RoundPosition(p)))
	{
		// 경로가 남아 있을 경우
		if (!mPath.empty())
		{
			glm::ivec2 lastPos = mPath[mPath.size() - 1];
			
			if (glm::distance(p, glm::vec2(lastPos)) < 2.0f 
				&& (terrain->IsUnitOnTile(RoundPosition(lastPos)) || terrain->IsObjectOnTile(RoundPosition(lastPos))))
			{
				glm::ivec2 closePos;
				if (! terrain->GetClosedPosition(lastPos, &closePos))
				{
					assert(0);
				}

				SetPath(terrain->GetPath(RoundPosition(p), closePos));
			}
			else
			{
				if (mPath[0] != RoundPosition(p))
				{
					SetPath(terrain->GetPath(RoundPosition(p), lastPos));
				}
			}
		}
		else
		{
			glm::ivec2 closePos;
			if (!terrain->GetClosedPosition(RoundPosition(mMovePos), &closePos))
			{
				assert(0);
			}
			SetPath(terrain->GetPath(RoundPosition(mMovePos), closePos));
		}
	}
	*/
	terrain->SetUnitOnTile(RoundPosition(p));
	SetPosition(glm::vec3(p.x, 0.0f, -p.y));
	SetHeight(terrain);

}
