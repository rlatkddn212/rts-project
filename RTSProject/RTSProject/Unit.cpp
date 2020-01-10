#include "Precompiled.h"
#include "Unit.h"
#include "Math3D.h"

using namespace std;

Unit::Unit(shared_ptr<UnitInfo> unitInfo)
{
	mUnitInfo = unitInfo;
	mIsSelect = false;
	mHealth = mUnitInfo->GetHealth();
	mAttackspeed = mUnitInfo->GetAttackSpeed();
	mRange = mUnitInfo->GetAttackRange();
	mDefense = mUnitInfo->GetDefense();
	mSpeed = mUnitInfo->GetSpeed();
	mDamege = mUnitInfo->GetDamege();

	mMoveComponent = nullptr;

	mSkinnedMesh = std::make_shared<SkinnedMesh>();
	mSkinnedMesh->LoadModel(mUnitInfo->GetModel());
	MakeBoxObject();
}

Unit::~Unit()
{

}

void Unit::MakeBoxObject()
{
	mBoxObject = std::make_shared<BoxObject>(mSkinnedMesh->GetMinPos(), mSkinnedMesh->GetMaxPos());
	mBoxObject->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
	SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
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
	
	if (mMoveComponent)
		mMoveComponent->Update(deltaTime);
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
	mSkinnedMesh->mAnimTime = 0.0f;
	mPathIdx = 0;
	mPath.clear();
	mPath = path;
}

void Unit::SetMove(std::shared_ptr<Terrain> terrain, glm::ivec2 movePos)
{
	if (!terrain->IsMovableTile(movePos) || terrain->IsObjectOnTile(movePos))
	{
		terrain->GetClosedPosition(movePos, &movePos);
	}

	vector<glm::ivec2> ret = terrain->GetPath(RoundPosition(glm::vec2(mPos.x, -mPos.z)), movePos);
	SetPath(ret);

	if (!ret.empty())
	{
		mMovePos = movePos;
	}
}

void Unit::InitPosOnTerrain(shared_ptr<Terrain> terrain, glm::vec2 p)
{
	terrain->SetTileState(RoundPosition(p), TileState::StaticObject);
	SetPosition(glm::vec3(p.x, 0.0f, -p.y));
	SetHeight(terrain);
}

void Unit::AttachMoveComponent(std::shared_ptr<MoveController> moveControl)
{
	mMoveComponent = moveControl;
}

void Unit::AttackObjectCommand(shared_ptr<RTSObject> obj)
{
	mUnitCommand = UNITCOMMAND_ATTACK_OBJECT;
}

void Unit::SetAttackCommand(std::shared_ptr<Terrain> terrain, glm::ivec2 pos)
{
	SetMove(terrain, pos);
	mUnitCommand = UNITCOMMAND_ATTACK_MOVE;
}

void Unit::SetPatrolCommand(std::shared_ptr<Terrain> terrain, glm::ivec2 pos)
{
	SetMove(terrain, pos);
	mUnitCommand = UNITCOMMAND_PATROL;
}

void Unit::SetStopCommand()
{
	mUnitCommand = UNITCOMMAND_STOP;
}

void Unit::SetHoldCommand()
{
	mUnitCommand = UNITCOMMAND_HOLD;
}