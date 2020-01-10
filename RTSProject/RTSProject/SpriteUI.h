#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include <string>

class SpriteUI
{
public:
										SpriteUI();
									   ~SpriteUI();

	void								SetScale(glm::vec2 scale) { mScaleX = scale.x; mScaleY = scale.y; }
	void								SetPos(glm::vec2 pos) { mPosX = pos.x; mPosY = pos.y;}
	void								SetTexture(const std::string& name);
	void								Render(std::shared_ptr<Camera> camera);

	void								SetUV(float x1, float y1, float x2, float y2);

private:
	float								mScaleX, mScaleY;
	float								mPosX, mPosY;
	std::shared_ptr<VertexArray>		mSpriteVerts;
	std::shared_ptr<Texture>			mTexture;
	std::shared_ptr<Shader>				mSpriteShader;
};

