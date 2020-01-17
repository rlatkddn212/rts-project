#pragma once

#include "Singleton.h"
class ObjectResourcePool : public Singleton<ObjectResourcePool>
{
	void								Initialize();
	void								Load();
};