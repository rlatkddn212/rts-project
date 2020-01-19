#pragma once

#include "Precompiled.h"
#include <string>
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Font.h"
#include "Camera.h"
#include "RenderObject.h"

class TextUI : public RenderObject
{
public:
											TextUI(const std::string& str);
										   ~TextUI();

	void									MakeModel();	
	void									Update();

	void									Render(std::shared_ptr<Camera> camera);
	void									AddRender(std::shared_ptr<Camera> camera);
	
private:
	std::string								mStr;
	float									mScale;
	int										mPosX, mPosY;

	std::shared_ptr<Texture>				mTexture;
	std::shared_ptr<Font>					mFont;
	std::shared_ptr<VertexArray>			mSpriteVerts;
	std::shared_ptr<Shader>					mSpriteShader;
};