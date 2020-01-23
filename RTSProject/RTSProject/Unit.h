#pragma once

#include "RTSObject.h"
#include "SkinnedMesh.h"
#include "BoxObject.h"
#include "Terrain.h"
#include "MoveComponent.h"
#include "RTSObjectInfo.h"
#include "UnitState.h"
#include "Effect.h"

class Effect;
class MoveComponent;

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
	UNITCOMMAND_BUILD
};
class UnitState;

class Unit : public RTSObject
{
public:
											Unit() {}
											Unit(std::shared_ptr<RTSObjectInfo> unitInfo);
										   ~Unit();
	
	void									MakeModel(std::string& model);
	std::shared_ptr<SkinnedMesh>			GetMesh() { return mSkinnedMesh; };
	std::shared_ptr<BoxObject>				GetBoxMesh() { return mBoxObject; };

	void									SetPosition(glm::vec3 p) { mPos = p; }
	void									SetRotation(glm::vec3 r) { mRot = r; }
	void									SetScale(glm::vec3 s) { mSca = s; }

	glm::vec3								GetDirection(glm::vec2 p1, glm::vec2 p2);
	void									SetHeight(std::shared_ptr<Terrain> terrain) { mPos.y = terrain->GetHeight(glm::vec2(mPos.x, mPos.z)); SetPosition(mPos); }
	void									Update(float deltaTime);
	virtual void							RenderShadow(std::shared_ptr<Camera> camera);
	void									Render(std::shared_ptr<Camera> camera);
	void									AddRender(std::shared_ptr<Camera> camera);

	void									Select();
	void									UnSelect();

	bool									Intersect(Ray ray);
	glm::vec2								GetScreenPos(std::shared_ptr<Camera> camera);
	
	bool									IsSelected() { return mIsSelect; }

	void									SetPath(const std::vector<glm::ivec2>& path);
	std::vector<glm::ivec2>					GetPath() { return mPath; }

	void									SetMove(std::shared_ptr<Terrain> terrain, glm::ivec2 movePos);
	glm::ivec2								GetMove() { return mMovePos; }

	void									SetAnimationSpeed(float speed) { mAnimationSpeed = speed; }
	void									SetAnimation(int idx);
	void									InitPosOnTerrain(std::shared_ptr<Terrain> terrain, glm::vec2 p);
	double									GetSpeed() { return mSpeed; }

	int										GetPathIdx() { return mPathIdx; }
	void									SetPathIdx(int pathIdx) { mPathIdx = pathIdx; }

	void									AttachMoveComponent(std::shared_ptr<MoveComponent> moveControl);

	void									AttackObjectCommand(std::shared_ptr<RTSObject> obj);
	void									SetMoveCommand(std::shared_ptr<Terrain> terrain, glm::ivec2 movePos);
	void									SetAttackCommand(std::shared_ptr<Terrain> terrain, glm::ivec2 pos);
	void									SetPatrolCommand(std::shared_ptr<Terrain> terrain, glm::ivec2 pos);
	void									SetStopCommand();
	void									SetHoldCommand();

	UnitCommand								GetCommand() { return mUnitCommand; }
	void									SetCommand(UnitCommand command) { mUnitCommand = command; }

	void									SetColor(glm::vec4 color) { mColor = color; }

	bool									FindEnemyInRange(std::shared_ptr<Terrain> terrain, std::vector<std::weak_ptr<RTSObject> >& mEnemy);

	void									SetTarget(std::shared_ptr<RTSObject> unit) { mTargetUnit = unit; }
	std::shared_ptr<RTSObject>				GetTarget() { return mTargetUnit.lock(); }

	void									SetState(std::shared_ptr<UnitState> state) { mUnitState = state; }
	std::shared_ptr<UnitState>				GetState() { return mUnitState; }

	void									AddEffect(std::shared_ptr<Effect> effect) { mEffect.push_back(effect); }

	virtual void							TakeDamege(double damege);
	bool									IsDead() { return mHealth <= 0; }
	bool									IsAnimationEnd() { return mIsAniEnd; }
	
	glm::ivec2								GetAttackPos() { return mAttackPos; }

protected:
	bool									mIsSelect;
	std::shared_ptr<BoxObject>				mBoxObject;
	int										mPathIdx;
	std::vector<glm::ivec2>					mPath;
	glm::ivec2								mMovePos;
	glm::ivec2								mAttackPos;
	
	std::shared_ptr<SkinnedMesh>			mSkinnedMesh;
	std::shared_ptr<MoveComponent>			mMoveComponent;
	
	std::shared_ptr<RTSObjectInfo>			mUnitInfo;
	std::shared_ptr<UnitState>				mUnitState;
	UnitCommand								mUnitCommand;
	
	int										mPatrolCount;
	glm::ivec2								mPatrolPos[2];
	std::weak_ptr<RTSObject>				mTargetUnit;
	std::vector<std::shared_ptr<Effect> >	mEffect;
	glm::vec4								mColor;

	int										mAnimationIdx;
	double									mAnimationSpeed;
	bool									mIsAniEnd;
	float									mAnimTime;
};