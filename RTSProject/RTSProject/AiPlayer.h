#pragma once
#include "Player.h"
#include "MasterAI.h"

class AiPlayer : public Player
{
public:
											AiPlayer();
	virtual								   ~AiPlayer();

	virtual void							Initialize(std::shared_ptr<Terrain> terrain, int w, int h);
	virtual void							Terminate();
protected:

	std::shared_ptr<MasterAI>				mMasterAI;
};

