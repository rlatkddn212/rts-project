#include "Precompiled.h"
#include "MoveController.h"
#include "Math3D.h"

using namespace std;

MoveController::MoveController()
{
}

MoveController::~MoveController()
{
}

void MoveController::SetTerrain(std::shared_ptr<Terrain> terrain)
{
	mTerrain = terrain;
}

void MoveController::SetUnit(std::shared_ptr<Unit> units)
{
	mUnit = units;
}

void MoveController::Update(float deltaTime)
{
	if (mUnit == nullptr) return;
	
	if (! mUnit->GetPath().empty())
	{
		mUnit->SetAnimation(2);
		float len = deltaTime * mUnit->GetSpeed();
		glm::vec3 pos = mUnit->GetPosition();

		//mTerrain->SetTileState(RoundPosition(glm::vec2(pos.x, -pos.z)), TileState::MoveObject);
		glm::vec2 newPos = MoveUnit(mUnit, len);

		// 길이 막힌 경우
		if (RoundPosition(glm::vec2(pos.x, -pos.z)) != RoundPosition(newPos)  && mTerrain->IsObjectOnTile(RoundPosition(newPos)))
		{
			// 막는 애가 이동 중이라면 잠시 기다린다.
			if (mTerrain->GetTileState(RoundPosition(newPos)) == TileState::MoveObject)
			{
				glm::vec3 pos = mUnit->GetPosition();
				mTerrain->SetTileState(RoundPosition(glm::vec2(pos.x, -pos.z)), TileState::StopObject);
				mUnit->SetAnimation(1);

				return;
			}
			// 도착했는데 막힌 경우 새로운 위치를 받는다.
			else if (glm::distance(newPos, glm::vec2(mUnit->GetMove())) < 1.0f)
			{
				glm::ivec2 closePos;
				if (!mTerrain->GetClosedPosition(mUnit->GetMove(), &closePos))
				{
					assert(0);
				}

				//unit->SetPath(mTerrain->GetPath(RoundPosition(glm::vec2(pos.x, -pos.z)), closePos));
				mUnit->SetMove(mTerrain, closePos);
			}
			else
			{
				// 이동중 막힌 경우
				const std::vector<glm::ivec2>& path = mTerrain->GetPath(RoundPosition(glm::vec2(pos.x, -pos.z)), mUnit->GetMove());
				if (!path.empty())
				{
					mUnit->SetPath(path);
				}
			}
				
			newPos = MoveUnit(mUnit, len);
		}

		if (glm::distance(newPos, glm::vec2(mUnit->GetMove())) < 0.1f)
		{
			mUnit->SetPath(vector<glm::ivec2>());
		}

		mUnit->SetPosition(glm::vec3(newPos.x, mTerrain->GetHeight(glm::vec2(newPos.x, -newPos.y)), -newPos.y));
		mTerrain->SetTileState(RoundPosition(glm::vec2(pos.x, -pos.z)), TileState::None);
		mTerrain->SetTileState(RoundPosition(newPos), TileState::MoveObject);
	}
	else
	{
		glm::vec3 pos = mUnit->GetPosition();
		mTerrain->SetTileState(RoundPosition(glm::vec2(pos.x, -pos.z)), TileState::StopObject);
		mUnit->SetAnimation(1);
	}
}

glm::vec2 MoveController::MoveUnit(shared_ptr<Unit> unit, float len)
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
