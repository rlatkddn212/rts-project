#pragma once

#include "StaticMesh.h"

class Building : public StaticMesh
{
public:
											Building(int type);
										   ~Building();
	virtual void							LoadShader();

	int										GetType() { return mType; }

private:
	int										mType;
};