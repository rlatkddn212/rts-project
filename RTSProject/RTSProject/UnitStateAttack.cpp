#include "Precompiled.h"
#include "UnitStateAttack.h"
#include "UnitStateGotoObj.h"
#include "UnitStateNone.h"
#include "UnitStateDie.h"

using namespace std;

// 공격 중일 때
void UnitStateAttack::Update(Unit * ownUnit, float deltaTime)
{
	ownUnit->SetAnimation(3);
	shared_ptr<Unit> target = dynamic_pointer_cast<Unit>(ownUnit->GetTarget());
	glm::vec3 pos1 = ownUnit->GetPosition();

	if (target)
	{
		shared_ptr<UnitStateDie> targetDie = dynamic_pointer_cast<UnitStateDie>(target->GetState());
		if (targetDie)
		{
			ownUnit->SetState(std::make_shared<UnitStateNone>());
		}
		else
		{
			glm::vec3 pos2 = target->GetPosition();
			float dist = glm::distance(pos1, pos2);

			// 범위를 벗어날 경우
			if (ownUnit->GetRange() < dist)
			{
				ownUnit->SetState(std::make_shared<UnitStateGotoObj>());
			}
			else
			{
				ownUnit->SetAnimation(3);
			}
		}
	}
}

bool UnitStateAttack::FindEnemyInRange(Unit * ownUnit, std::shared_ptr<Terrain> terrain, std::vector<std::weak_ptr<RTSObject>>& mEnemy, UnitCommand command)
{
	return false;
}
