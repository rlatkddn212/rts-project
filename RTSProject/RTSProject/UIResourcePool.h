#pragma once

#include "Singleton.h"
class UIResourcePool : public Singleton<UIResourcePool>
{
	void								Initialize();
	void								Load();
};

