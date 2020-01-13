#include "Precompiled.h"
#include "UnitStateAttack.h"
#include "UnitStateGotoObj.h"
#include "UnitStateNone.h"
#include "UnitStateDie.h"
#include "Effect.h"

using namespace std;

// 공격 중일 때
void UnitStateAttack::Update(Unit * ownUnit, float deltaTime)
{
	ownUnit->SetAnimation(3);
	shared_ptr<Unit> target = dynamic_pointer_cast<Unit>(ownUnit->GetTarget());
	glm::vec3 pos1 = ownUnit->GetPosition();

	if (target)
	{
		double time = ownUnit->GetAttackTime();
		if (time == 0.0)
		{
			ownUnit->SetAnimationSpeed(ownUnit->GetAttackSpeed());

			std::shared_ptr<EffectFireBall> ball = std::make_shared<EffectFireBall>(pos1, target, ownUnit);
			ownUnit->AddEffect(ball);
			ownUnit->SetAttackTime(time + deltaTime);
		}
		else if (time < ownUnit->GetAttackSpeed())
		{
			ownUnit->SetAttackTime(time + deltaTime);
		}
		else
		{
			ownUnit->SetAttackTime(0.0);
		}

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
