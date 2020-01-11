#include "Precompiled.h"
#include "UnitStateGotoObj.h"
#include "UnitStateAttack.h"
#include "UnitStateDie.h"
#include "UnitStateNone.h"
using namespace std;

// 공격 가능한 오브젝트에게 달려간다.
void UnitStateGotoObj::Update(Unit * ownUnit, float deltaTime)
{
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
}

bool UnitStateGotoObj::FindEnemyInRange(Unit * ownUnit, std::shared_ptr<Terrain> terrain, std::vector<std::weak_ptr<RTSObject>>& mEnemy, UnitCommand command)
{
	// 멈춰 있거나, 이동 공격, 패트롤 중에 적을 발견한 경우
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
			if (dist < ownUnit->GetRange())
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
		glm::vec3 tempPos = tempUnit->GetPosition();
		ownUnit->SetState(std::make_shared<UnitStateAttack>());
		ownUnit->SetPath(vector<glm::ivec2>());
		
		return true;
	}
	// 공격 범위에 타겟이 없음
	else
	{
		ownUnit->SetState(std::make_shared<UnitStateNone>());
	}

	return false;
}
