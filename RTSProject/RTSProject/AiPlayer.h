#pragma once
#include "Player.h"
#include "MasterAI.h"

class AiPlayer : public Player
{
public:
											AiPlayer();
	virtual								   ~AiPlayer();

protected:

	std::shared_ptr<MasterAI>				mMasterAI;
};

