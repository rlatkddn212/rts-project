#pragma once

#include "Precompiled.h"
#include "Unit.h"

class FogOfWar
{
public:
											FogOfWar();

	virtual	void							Update(float deltaTime, std::vector<std::shared_ptr<Unit> > unit);
	virtual void							Render();

	GLuint									mVao;
	GLuint									mSightTexture;
	GLuint									mVisiableTexture;
	GLuint									mVisitedTexture;
	GLuint									mFogTexture;

};

