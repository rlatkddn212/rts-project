#pragma once

#include "Precompiled.h"
#include "Texture.h"
#include "Unit.h"

class FogOfWar
{
public:
											FogOfWar(int w, int h);

	virtual	void							Update(float deltaTime, std::vector<std::shared_ptr<Unit> > unit);
	virtual void							Render();
	
	void									PrintScreen(GLuint framebuffer, const std::string& str);
	std::shared_ptr<Texture>				GetFogTexture() { return mFogTexture; }

	GLuint									mVao;

	int										mWidth;
	int										mHeight;

	Shader									mVisibleShader;
	Shader									mVisitedShader;
	Shader									mFogShader;

	std::shared_ptr<Texture>				mSightTexture;
	std::shared_ptr<Texture>				mVisibleTexture;
	std::shared_ptr<Texture>				mVisitedTexture;
	std::shared_ptr<Texture>				mFogTexture;
};

