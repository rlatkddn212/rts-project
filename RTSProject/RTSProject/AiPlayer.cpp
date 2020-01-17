#include "Precompiled.h"
#include "AiPlayer.h"

AiPlayer::AiPlayer()
{
}

AiPlayer::~AiPlayer()
{
}

void AiPlayer::Initialize(std::shared_ptr<Terrain> terrain, int w, int h)
{
	Player::Initialize(terrain, w, h);

	for (int i = 0; i < mUnits.size(); ++i)
	{
		mUnits[i]->SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}
}

void AiPlayer::Terminate()
{
}
