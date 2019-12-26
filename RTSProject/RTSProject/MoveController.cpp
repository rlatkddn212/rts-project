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

void MoveController::SetUnit(std::vector<std::shared_ptr<Unit>>& units)
{
	mpUnits = &units;
}

void MoveController::Update(float deltaTime)
{
	if (mpUnits == nullptr) return;
	for (int unitIdx = 0; unitIdx < mpUnits->size(); ++unitIdx)
	{
		if ((*mpUnits)[unitIdx] == nullptr) continue;

		shared_ptr<Unit> unit = (*mpUnits)[unitIdx];
		if (! unit->GetPath().empty())
		{
			unit->SetAnimation(2);
			float len = deltaTime * unit->GetSpeed();
			glm::vec3 pos = unit->GetPosition();

			mTerrain->SetUnitOnTile(RoundPosition(glm::vec2(pos.x, -pos.z)), -1);
			glm::vec2 newPos = MoveUnit(unit, len);

			// 길이 막힌 경우
			if (mTerrain->IsUnitOnTile(RoundPosition(newPos)) || mTerrain->IsObjectOnTile(RoundPosition(newPos)))
			{
				// 도착했는데 막힌 경우 새로운 위치를 받는다.
				
				if (glm::distance(newPos, glm::vec2(unit->GetMove())) < 1.0f)
				{
					glm::ivec2 closePos;
					if (!mTerrain->GetClosedPosition(unit->GetMove(), &closePos))
					{
						assert(0);
					}

					//unit->SetPath(mTerrain->GetPath(RoundPosition(glm::vec2(pos.x, -pos.z)), closePos));
					unit->SetMove(mTerrain, closePos);
					mTerrain->SetUnitOnTile(RoundPosition(closePos));
				}
				else
				{
					// 이동중 막힌 경우
					const std::vector<glm::ivec2>& path = mTerrain->GetPath(RoundPosition(glm::vec2(pos.x, -pos.z)), unit->GetMove());
					if (!path.empty())
					{
						unit->SetPath(path);
					}
				}
				
				newPos = MoveUnit(unit, len);
			}

			if (RoundPosition(glm::vec2(newPos.x, newPos.y)) == unit->GetMove())
			{
				unit->SetPath(vector<glm::ivec2>());
			}

			unit->SetPosition(glm::vec3(newPos.x, mTerrain->GetHeight(glm::vec2(newPos.x, -newPos.y)), -newPos.y));
			mTerrain->SetUnitOnTile(RoundPosition(newPos));
		}
		else
		{
			glm::vec3 pos = unit->GetPosition();
			mTerrain->SetUnitOnTile(RoundPosition(glm::vec2(pos.x, -pos.z)));
			unit->SetAnimation(1);
		}
	}
}

glm::vec2 MoveController::MoveUnit(shared_ptr<Unit> unit, float len)
{
	glm::vec3 unitPos = unit->GetPosition();
	glm::vec2 prev = glm::vec2(unitPos.x, -unitPos.z);
	vector<glm::ivec2> mPath = unit->GetPath();

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
