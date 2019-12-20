#pragma once

#include "Precompiled.h"
#include <string>
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Font.h"
#include "Camera.h"

class TextUI
{
public:
										TextUI();
									   ~TextUI();

	void								SetText(const std::string& str);
	void								Update();
	void								Render(std::shared_ptr<Camera> camera);
	
	float								mScale;
	int									mPosX, mPosY;
	std::shared_ptr<Texture>			mTexture;
	std::shared_ptr<Font>				mFont;
	std::shared_ptr<VertexArray>		mSpriteVerts;
	std::shared_ptr<Shader>				mSpriteShader;
};

