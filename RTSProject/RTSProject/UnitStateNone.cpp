#include "Precompiled.h"
#include "UnitStateNone.h"
#include "UnitStateGotoObj.h"

using namespace std;

void UnitStateNone::Update(Unit * ownUnit, float deltaTime)
{
	ownUnit->SetAnimation(1);
}

bool UnitStateNone::FindEnemyInRange(Unit * ownUnit, std::shared_ptr<Terrain> terrain, std::vector<std::weak_ptr<RTSObject>>& mEnemy, UnitCommand command)
{
	// ���� �̵�, ��Ʈ��, ���� ������ ��
	if (command == UnitCommand::UNITCOMMAND_ATTACK_MOVE ||
		command == UnitCommand::UNITCOMMAND_NONE ||
		command == UnitCommand::UNITCOMMAND_PATROL)
	{
		// ���� �ְų�, �̵� ����, ��Ʈ�� �߿� ���� �߰��� ���
		shared_ptr<RTSObject> tempUnit = nullptr;
		float minSight = 100.0f;
		glm::vec3 pos2 = ownUnit->GetPosition();

		for (size_t i = 0; i < mEnemy.size(); ++i)
		{
			shared_ptr<RTSObject> enemyUnit = mEnemy[i].lock();
			if (enemyUnit)
			{
				glm::vec3 pos1 = enemyUnit->GetPosition();

				float dist = glm::distance(pos1, pos2);
				if (dist < ownUnit->GetSight())
				{
					if (dist < minSight)
					{
						minSight = dist;
						tempUnit = enemyUnit;
					}
				}
			}
		}

		if (tempUnit)
		{
			glm::vec3 tempPos = tempUnit->GetPosition();
			ownUnit->SetMove(terrain, glm::ivec2(tempPos.x, -tempPos.z));
			ownUnit->SetState(make_shared<UnitStateGotoObj>());
			ownUnit->SetTarget(tempUnit);
			
			return true;
		}
	}

	return false;
}
