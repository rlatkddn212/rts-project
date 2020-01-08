#include "Precompiled.h"
#include "groupAI.h"
#include "masterAI.h"
#include "strategyMap.h"

using namespace std;

GroupAI::GroupAI()
{
}

GroupAI::~GroupAI()
{
}

void GroupAI::AddMember(std::shared_ptr<Unit> newMember)
{
}

void GroupAI::RemoveMember(std::shared_ptr<Unit> oldMember)
{
}

void GroupAI::ClearGroup()
{
}

void GroupAI::EnemiesSpotted(std::vector<std::shared_ptr<Unit>>& manyEnemies)
{
}

void GroupAI::SetAction(int newTask, SDL_FRect area)
{
}

shared_ptr<GroupAI> GroupAI::SplitGroup(std::vector<int> units)
{
	return nullptr;
}

void GroupAI::DoGroupAI()
{
}

bool GroupAI::IsDead()
{
	return false;
}

glm::ivec2 GroupAI::GetCenter()
{
	return glm::ivec2();
}

void GroupAI::Goto(SDL_FRect mArea)
{
}

void GroupAI::Attack(std::vector<std::shared_ptr<Unit>>& enemies)
{
}

void GroupAI::RetreatTo(SDL_FRect ma)
{
}

void GroupAI::Shuffle()
{
}
