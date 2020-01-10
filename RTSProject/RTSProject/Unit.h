#pragma once

#include "RTSObject.h"
#include "SkinnedMesh.h"
#include "BoxObject.h"
#include "Terrain.h"
#include "MoveController.h"
#include "UnitInfo.h"

class MoveController;

enum UnitCommand
{
	UNITCOMMAND_NONE,
	UNITCOMMAND_MOVE,
	UNITCOMMAND_ATTACK_OBJECT,
	UNITCOMMAND_ATTACK_MOVE,
	UNITCOMMAND_PATROL,
	UNITCOMMAND_STOP,
	UNITCOMMAND_HOLD,
	UNITCOMMAND_SKILL,
};

enum UnitState
{
	UNIT_NONE,
	UNIT_MOVE,
	UNIT_ATTACK,
	UNIT_SKILL,
	UNIT_DIE,
};

class Unit : public RTSObject
{
public:
											Unit(std::shared_ptr<UnitInfo> unitInfo);
										   ~Unit();
	
	void									MakeBoxObject();
	void									SetPosition(glm::vec3 p) { mPos = p;  mSkinnedMesh->SetPosition(p); mBoxObject->SetPosition(p); }
	void									SetRotation(glm::vec3 r) { mRot = r;  mSkinnedMesh->SetRotation(r); }
	void									SetScale(glm::vec3 s) { mSca = s; mSkinnedMesh->SetScale(s); }

	glm::vec3								GetDirection(glm::vec2 p1, glm::vec2 p2);
	glm::vec3 								GetPosition() { return mPos; }
	void									SetHeight(std::shared_ptr<Terrain> terrain) { mPos.y = terrain->GetHeight(glm::vec2(mPos.x, mPos.z)); SetPosition(mPos); }
	void									Update(float deltaTime);
	void									Render(std::shared_ptr<Camera> camera);

	void									Select();
	void									UnSelect();

	bool									Intersect(Ray ray);
	glm::vec2								GetScreenPos(std::shared_ptr<Camera> camera);
	
	bool									isSelected() { return mIsSelect; }

	void									SetPath(const std::vector<glm::ivec2>& path);
	std::vector<glm::ivec2>					GetPath() { return mPath; }

	void									SetMove(std::shared_ptr<Terrain> terrain, glm::ivec2 movePos);
	glm::ivec2								GetMove() { return mMovePos; }

	void									SetAnimation(int idx) { mSkinnedMesh->mAnimationIdx = idx; };
	void									InitPosOnTerrain(std::shared_ptr<Terrain> terrain, glm::vec2 p);
	float									GetSpeed() { return mSpeed; }

	int										GetPathIdx() { return mPathIdx; }
	void									SetPathIdx(int pathIdx) { mPathIdx = pathIdx; }

	void									AttachMoveComponent(std::shared_ptr<MoveController> moveControl);
	void									AttackObjectCommand(std::shared_ptr<RTSObject> obj);

	void									SetAttackCommand(std::shared_ptr<Terrain> terrain, glm::ivec2 pos);
	void									SetPatrolCommand(std::shared_ptr<Terrain> terrain, glm::ivec2 pos);
	void									SetStopCommand();
	void									SetHoldCommand();

protected:
	int										mHealth;
	int										mMaxHealth;
	double									mAttackspeed;
	double									mRange;
	double									mDefense;
	double									mSpeed;
	double									mDamege;

	bool									mIsSelect;
	std::shared_ptr<BoxObject>				mBoxObject;

	int										mPathIdx;
	std::vector<glm::ivec2>					mPath;


	std::shared_ptr<UnitInfo>				mUnitInfo;

	glm::ivec2								mMovePos;
	std::shared_ptr<SkinnedMesh>			mSkinnedMesh;

	std::shared_ptr<MoveController>			mMoveComponent;

	UnitCommand								mUnitCommand;

	glm::ivec2								mAttackPos;

	int										mPatrolCount;
	glm::ivec2								mPatrolPos[2];
};