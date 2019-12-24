#pragma once
#include "Precompiled.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

class HeightMap
{
public:
											HeightMap(glm::ivec2 size);
										   ~HeightMap();

	bool									LoadFromFile(const std::string& fileName);
	bool									CreateRandomHeightMap(int seed, float noiseSize, float persistence, int octaves);
	
	void									CreateParticles();

	void									Update(float deltaTime);
	void									Render(std::shared_ptr<Camera> camera);
	
	// 삼각형 상에 점 공식으로 변경 필요
	float									GetHeight(glm::ivec2 pos) { return mHeightMap[pos.y * mSize.x + pos.x]; }
	glm::ivec2								GetCentre() { return glm::ivec2(mSize.x / 2.0f, mSize.y / 2.0f); }

	glm::ivec2								mSize;
	float									mMaxHeight;
	float*									mHeightMap;
	
	float									mAngle;
	GLuint									mVertexArrayID;
	
	glm::mat4								mPerspect;
	glm::mat4								mView;

	unsigned int							mTexture;
	GLuint									mColorTexture;

	GLuint									mVertexbuffer;
	GLuint									mUvBuffer;

	std::shared_ptr<VertexArray>			mVertexArray;
	std::shared_ptr<Texture>				mHeightMapTexture;
	std::shared_ptr<Shader>					mShader;
};

