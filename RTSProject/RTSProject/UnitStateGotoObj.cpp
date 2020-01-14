#include "Precompiled.h"
#include "UnitStateGotoObj.h"
#include "UnitStateAttack.h"
#include "UnitStateDie.h"
#include "UnitStateNone.h"
using namespace std;

// 공격 가능한 오브젝트에게 달려간다.
void UnitStateGotoObj::Update(Unit * ownUnit, float deltaTime)
{
	ownUnit->SetAnimation(2);
	shared_ptr<Unit> target = dynamic_pointer_cast<Unit>(ownUnit->GetTarget());
	glm::vec3 pos1 = ownUnit->GetPosition();

	if (target)
	{
		shared_ptr<UnitStateDie> targetDie = dynamic_pointer_cast<UnitStateDie>(target->GetState());
		// 타겟이 죽었다면
		if (targetDie)
		{			
			ownUnit->SetState(std::make_shared<UnitStateNone>());
		}
		else
		{
			glm::vec3 pos2 = target->GetPosition();
			float dist = glm::distance(pos1, pos2);

			// 타겟이 시야를 벗어난 경우
			if (ownUnit->GetSight() < dist)
			{
				ownUnit->SetState(std::make_shared<UnitStateNone>());
			}
		}
	}
	else
	{
		ownUnit->SetState(std::make_shared<UnitStateNone>());
	}
}

bool UnitStateGotoObj::FindEnemyInRange(Unit * ownUnit, std::shared_ptr<Terrain> terrain, std::vector<std::weak_ptr<RTSObject>>& mEnemy, UnitCommand command)
{
	shared_ptr<RTSObject> tempUnit = nullptr;
	float minRange = 100.0f;
	glm::vec3 pos2 = ownUnit->GetPosition();

	for (int i = 0; i < mEnemy.size(); ++i)
	{
		shared_ptr<RTSObject> enemyUnit = mEnemy[i].lock();
		if (enemyUnit)
		{
			glm::vec3 pos1 = enemyUnit->GetPosition();

			float dist = glm::distance(pos1, pos2);
			if (dist < ownUnit->GetSight())
			{
				if (dist < minRange)
				{
					minRange = dist;
					tempUnit = enemyUnit;
				}
			}
		}
	}

	// 공격 범위에 타겟
	if (tempUnit)
	{
		if (minRange < ownUnit->GetRange())
		{
			glm::vec3 tempPos = tempUnit->GetPosition();
			ownUnit->SetAttackTime(0.0f);
			ownUnit->SetState(std::make_shared<UnitStateAttack>());
			ownUnit->SetPath(vector<glm::ivec2>());
		}
		else
		{
			glm::vec3 tempPos = tempUnit->GetPosition();
			ownUnit->SetMove(terrain, glm::ivec2(tempPos.x, -tempPos.z));
			if (ownUnit->GetPath().empty())
			{
				ownUnit->SetState(make_shared<UnitStateNone>());
			}
			else
			{
				ownUnit->SetState(make_shared<UnitStateGotoObj>());
				ownUnit->SetTarget(tempUnit);
			}
		}
		
		return true;
	}
	// 공격 범위에 타겟이 없음
	else
	{
		ownUnit->SetState(std::make_shared<UnitStateNone>());
	}

	return false;
}
