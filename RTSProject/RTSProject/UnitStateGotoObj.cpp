#include "Precompiled.h"
#include "UnitStateGotoObj.h"
#include "UnitStateAttack.h"
#include "UnitStateDie.h"
#include "UnitStateNone.h"
using namespace std;

// ���� ������ ������Ʈ���� �޷�����.
void UnitStateGotoObj::Update(Unit * ownUnit, float deltaTime)
{
	shared_ptr<Unit> target = dynamic_pointer_cast<Unit>(ownUnit->GetTarget());
	glm::vec3 pos1 = ownUnit->GetPosition();

	if (target)
	{
		shared_ptr<UnitStateDie> targetDie = dynamic_pointer_cast<UnitStateDie>(target->GetState());
		// Ÿ���� �׾��ٸ�
		if (targetDie)
		{
			ownUnit->SetState(std::make_shared<UnitStateNone>());
		}
		else
		{
			glm::vec3 pos2 = target->GetPosition();
			float dist = glm::distance(pos1, pos2);

			// Ÿ���� �þ߸� ��� ���
			if (ownUnit->GetSight() < dist)
			{
				ownUnit->SetState(std::make_shared<UnitStateNone>());
			}
		}
	}
}

bool UnitStateGotoObj::FindEnemyInRange(Unit * ownUnit, std::shared_ptr<Terrain> terrain, std::vector<std::weak_ptr<RTSObject>>& mEnemy, UnitCommand command)
{
	// ���� �ְų�, �̵� ����, ��Ʈ�� �߿� ���� �߰��� ���
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

	// ���� ������ Ÿ��
	if (tempUnit)
	{
		glm::vec3 tempPos = tempUnit->GetPosition();
		ownUnit->SetState(std::make_shared<UnitStateAttack>());
		ownUnit->SetPath(vector<glm::ivec2>());
		
		return true;
	}
	// ���� ������ Ÿ���� ����
	else
	{
		ownUnit->SetState(std::make_shared<UnitStateNone>());
	}

	return false;
}
