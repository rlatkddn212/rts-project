#pragma once
#include "Precompiled.h"
#include "Shader.h"
#include "Font.h"
#include "VertexArray.h"

class WindowGroup
{
public:
											WindowGroup();
										   ~WindowGroup();

	void									Initialize();
	void									Terminate();
	void									Update();
	void									Draw();
	void									DrawTexture(std::shared_ptr<Texture> texture, const glm::vec2& offset, float scale);

	void									DrawBackGround();

	std::shared_ptr<Font>					mFont;
	GLuint									VertexArrayID;
	GLuint									vertexbuffer;
	GLuint									uvBuffer;

	std::shared_ptr<Texture>				mTexture;
	std::shared_ptr<VertexArray>			mBackgroundVerts;
	std::shared_ptr<Shader>					mBackgroundShader;
	std::shared_ptr<VertexArray>			mSpriteVerts;
	std::shared_ptr<Shader>					mSpriteShader;
};
