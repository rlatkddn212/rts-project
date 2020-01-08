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

											// 그룹에 맴버 추가
	void									AddMember(std::shared_ptr<Unit> newMember);
											// 죽은 맴버 제거
	void									RemoveMember(std::shared_ptr<Unit> oldMember);
											// 그룹 맴버 모두 제거
	void									ClearGroup();

											// 적을 점사한다.
	void									EnemiesSpotted(std::vector<std::shared_ptr<Unit>> &manyEnemies);
											// 그룹에 행동 지정
	void									SetAction(int newTask, SDL_FRect area);
											// 그룹 분리
	std::shared_ptr<GroupAI>				SplitGroup(std::vector<int> units);

											// 그룹 AI 실행
	void									DoGroupAI();
	glm::ivec2								GetCenter();

											// 이동
	void									Goto(SDL_FRect area);
											// 공격
	void									Attack(std::vector<std::shared_ptr<Unit>>& enemies);

private:
	std::vector<std::shared_ptr<Unit>>		mMembers;
	std::vector<std::shared_ptr<Unit>>		mVisibleEnemies;
	int										mTask;
	int										mState;
};
