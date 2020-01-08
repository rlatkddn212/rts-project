#pragma once

#include <vector>
#include "mapObject.h"
#include "Unit.h"

enum GROUP_ACTION
{
	GROUP_NONE,
	GROUP_DEFEND,
	GROUP_ATTACK,
	GROUP_SCOUT
};

class GroupAI
{
public:
											GroupAI();
											~GroupAI();

											// �׷쿡 �ɹ� �߰�
	void									AddMember(std::shared_ptr<Unit> newMember);
											// ���� �ɹ� ����
	void									RemoveMember(std::shared_ptr<Unit> oldMember);
											// �׷� �ɹ� ��� ����
	void									ClearGroup();

											// ���� �����Ѵ�.
	void									EnemiesSpotted(std::vector<std::shared_ptr<Unit>> &manyEnemies);
											// �׷쿡 �ൿ ����
	void									SetAction(int newTask, SDL_FRect area);
											// �׷� �и�
	std::shared_ptr<GroupAI>				SplitGroup(std::vector<int> units);

											// �׷� AI ����
	void									DoGroupAI();
	glm::ivec2								GetCenter();

											// �̵�
	void									Goto(SDL_FRect area);
											// ����
	void									Attack(std::vector<std::shared_ptr<Unit>>& enemies);

private:
	std::vector<std::shared_ptr<Unit>>		mMembers;
	std::vector<std::shared_ptr<Unit>>		mVisibleEnemies;
	int										mTask;
	int										mState;
};
