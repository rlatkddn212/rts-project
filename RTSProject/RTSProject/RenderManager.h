#pragma once
#include "Singleton.h"

class RenderManager : public Singleton<RenderManager>
{
	void									AddQueue();
	//std::vector<std::shared_ptr<Mesh>>		GetQueue();
	void									Render();
};
