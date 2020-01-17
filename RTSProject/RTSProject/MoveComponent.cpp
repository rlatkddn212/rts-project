#include "Precompiled.h"
#include "MoveComponent.h"
#include "Math3D.h"
#include "UnitStateNone.h"

using namespace std;

MoveComponent::MoveComponent()
{
}

MoveComponent::~MoveComponent()
{
}

void MoveComponent::SetTerrain(std::shared_ptr<Terrain> terrain)
{
	mTerrain = terrain;
}

void MoveComponent::SetUnit(std::shared_ptr<Unit> units)
{
	mUnit = units;
}

void MoveComponent::Update(float deltaTime)
{
	shared_ptr<Unit> moveUnit = mUnit.lock();
	if (moveUnit == nullptr) return;
	
	if (! moveUnit->GetPath().empty())
	{
		float len = deltaTime * moveUnit->GetSpeed();
		glm::vec3 pos = moveUnit->GetPosition();

		glm::vec2 newPos = MoveUnit(moveUnit, len);

		// 길이 막힌 경우
		if (RoundPosition(glm::vec2(pos.x, -pos.z)) != RoundPosition(newPos)  && mTerrain->IsObjectOnTile(RoundPosition(newPos)))
		{
			// 막는 애가 이동 중이라면 잠시 기다린다.
			if (mTerrain->GetTileState(RoundPosition(newPos)) == TileState::MoveObject)
			{
				glm::vec3 pos = moveUnit->GetPosition();
				mTerrain->SetTileState(RoundPosition(glm::vec2(pos.x, -pos.z)), TileState::StopObject);

				return;
			}
			// 도착했는데 막힌 경우 새로운 위치를 받는다.
			else if (glm::distance(newPos, glm::vec2(moveUnit->GetMove())) < 1.0f)
			{
				glm::ivec2 closePos;
				if (!mTerrain->GetClosedPosition(moveUnit->GetMove(), &closePos))
				{
					assert(0);
				}

				moveUnit->SetMove(mTerrain, closePos);
			}
			else
			{
				// 이동중 막힌 경우
				const std::vector<glm::ivec2>& path = mTerrain->GetPath(RoundPosition(glm::vec2(pos.x, -pos.z)), moveUnit->GetMove());
				if (!path.empty())
				{
					moveUnit->SetPath(path);
				}
			}
				
			newPos = MoveUnit(moveUnit, len);
		}

		if (glm::distance(newPos, glm::vec2(moveUnit->GetMove())) < 0.1f)
		{
			Arrive();
		}

		moveUnit->SetPosition(glm::vec3(newPos.x, mTerrain->GetHeight(glm::vec2(newPos.x, -newPos.y)), -newPos.y));
		mTerrain->SetTileState(RoundPosition(glm::vec2(pos.x, -pos.z)), TileState::None);
		mTerrain->SetTileState(RoundPosition(newPos), TileState::MoveObject);
	}
	else
	{
		glm::vec3 pos = moveUnit->GetPosition();
		mTerrain->SetTileState(RoundPosition(glm::vec2(pos.x, -pos.z)), TileState::StopObject);
	}
}

glm::vec2 MoveComponent::MoveUnit(shared_ptr<Unit> unit, float len)
{
	glm::vec3 unitPos = unit->GetPosition();
	glm::vec2 prev = glm::vec2(unitPos.x, -unitPos.z);
	vector<glm::ivec2> mPath = unit->GetPath();

	if (mPath.empty())
	{
		return prev;
	}

	int pathIdx = unit->GetPathIdx();
	for (int i = pathIdx; i < mPath.size(); ++i)
	{
		float d = glm::distance(prev, glm::vec2(mPath[i]));
		len -= d;
		if (len < 0.0f)
		{
			glm::vec2 pos = prev + (1 - (-len / d)) * (glm::vec2(mPath[i]) - prev);
			unit->SetRotation(unit->GetDirection(glm::vec2(mPath[i]), prev));
			unit->SetPathIdx(pathIdx);
			return pos;
		}

		++pathIdx;
		prev = mPath[i];
	}

	return glm::vec2(mPath[mPath.size() - 1]);
}

void MoveComponent::Arrive()
{
	shared_ptr<Unit> moveUnit = mUnit.lock();
	moveUnit->SetPath(vector<glm::ivec2>());

	// 도달 명령들을 완료한다.
	switch (moveUnit->GetCommand())
	{
	case UNITCOMMAND_ATTACK_MOVE:
	case UNITCOMMAND_MOVE:
		moveUnit->SetCommand(UNITCOMMAND_NONE);
		moveUnit->SetState(std::make_shared<UnitStateNone>());

		break;
	}
}
