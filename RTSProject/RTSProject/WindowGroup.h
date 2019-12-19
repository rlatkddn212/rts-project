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

	void Initialize();
	void Terminate();
	void Update();
	void Draw();
	void DrawTexture(class Texture* texture, const glm::vec2& offset, float scale);

	void DrawBackGround();

	Font* mFont;
	GLuint VertexArrayID;
	GLuint vertexbuffer;
	GLuint uvBuffer;
	Texture* t;
	VertexArray* mBackgroundVerts;
	Shader* mBackgroundShader;
	VertexArray* mSpriteVerts;
	Shader* mSpriteShader;
};
