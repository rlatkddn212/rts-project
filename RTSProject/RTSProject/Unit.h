#pragma once

#include "SkinnedMesh.h"
#include "BoxObject.h"

class Unit
{
public:
											Unit();
										   ~Unit();
	
	void									MakeBoxObject();
	void									SetPosition(glm::vec3 p) { mPos = p;  mSkinnedMesh->SetPosition(p); mBoxObject->SetPosition(p); }
	void									SetRotation(glm::vec3 r) { mRot = r;  mSkinnedMesh->SetRotation(r); }
	void									SetScale(glm::vec3 s) { mSca = s; mSkinnedMesh->SetScale(s); }
	
	void									Update(float deltaTime);
	void									Render(std::shared_ptr<Camera> camera);

	void									Select();
	void									UnSelect();

	bool									Intersect(Ray ray);
	glm::vec2								GetScreenPos(std::shared_ptr<Camera> camera);
	
	bool									isSelected() { return mIsSelect; }

	bool									mIsSelect;
	std::shared_ptr<BoxObject>				mBoxObject;

	glm::vec3								mPos, mRot, mSca;
	
	std::shared_ptr<SkinnedMesh>			mSkinnedMesh;
};