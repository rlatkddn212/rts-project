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

	void								SetPos(int x, int y);
	void								SetTexture(std::string& name);
	void								Render(Camera* camera);

private:
	int									mPosX, mPosY;
	std::shared_ptr<VertexArray>		mSpriteVerts;
	std::shared_ptr<Texture>			mTexture;
	std::shared_ptr<Shader>				mSpriteShader;
};
