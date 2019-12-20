#pragma once
#include "Precompiled.h"
#include "Shader.h"
#include "Font.h"
#include "VertexArray.h"
#include "TextUI.h"
#include "Camera.h"

class WindowGroup
{
public:
											WindowGroup();
										   ~WindowGroup();

	void									Initialize();
	void									Terminate();
	void									Update();
	void									Render(std::shared_ptr<Camera> camera);

	void									DrawBackGround();
	
	GLuint									VertexArrayID;
	GLuint									vertexbuffer;
	GLuint									uvBuffer;

	std::shared_ptr<TextUI>					mText;
	std::shared_ptr<Texture>				mTexture;
	std::shared_ptr<VertexArray>			mBackgroundVerts;
	std::shared_ptr<Shader>					mBackgroundShader;
};
