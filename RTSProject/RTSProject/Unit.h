#pragma once

#include "SkinnedMesh.h"
#include "BoxObject.h"
#include "Terrain.h"

class Unit
{
public:
											Unit();
										   ~Unit();
	
	void									MakeBoxObject();
	void									SetPosition(glm::vec3 p) { mPos = p;  mSkinnedMesh->SetPosition(p); mBoxObject->SetPosition(p); }
	void									SetRotation(glm::vec3 r) { mRot = r;  mSkinnedMesh->SetRotation(r); }
	void									SetScale(glm::vec3 s) { mSca = s; mSkinnedMesh->SetScale(s); }

	glm::vec3								GetDirection(glm::vec2 p1, glm::vec2 p2);
	glm::vec3 								GetPosition() { return mPos; }
	void									SetHeight(std::shared_ptr<Terrain> terrain) { mPos.y = terrain->GetHeight(mPos.x, mPos.z); SetPosition(mPos); }
	void									Update(float deltaTime);
	void									Render(std::shared_ptr<Camera> camera);

	void									Select();
	void									UnSelect();

	bool									Intersect(Ray ray);
	glm::vec2								GetScreenPos(std::shared_ptr<Camera> camera);
	
	bool									isSelected() { return mIsSelect; }

	void									SetPath(const std::vector<glm::ivec2>& path);
	glm::ivec2								GetNextPos() { mPosIndex += 1; return mPath[mPosIndex]; }

	void									SetAnimation(int idx) { mSkinnedMesh->mAnimationIdx = idx; };

	void									SetPosOnTerrain(std::shared_ptr<Terrain> terrain, float x, float y);

	bool									mIsSelect;
	std::shared_ptr<BoxObject>				mBoxObject;

	int										mPosIndex;

	glm::vec3								mPos, mRot, mSca;
	std::vector<glm::ivec2>					mPath;

	float									speed;

	std::shared_ptr<SkinnedMesh>			mSkinnedMesh;
};