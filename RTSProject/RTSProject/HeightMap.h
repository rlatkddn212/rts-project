#pragma once
#include "Precompiled.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Shader.h"

class HeightMap
{
public:
	HeightMap(glm::ivec2 size);
	~HeightMap();

	bool LoadFromFile(const std::string& fileName);
	bool CreateRandomHeightMap(int seed, float noiseSize, float persistence, int octaves);
	
	void CreateParticles();

	void Update(float deltaTime);
	void Render();
	
	float GetHeight(glm::ivec2 pos) { return mHeightMap[pos.y * mSize.x + pos.x]; }
	glm::ivec2 GetCentre() { return glm::ivec2(mSize.x / 2.0f, mSize.y / 2.0f); }

	glm::ivec2 mSize;
	float mMaxHeight;
	float* mHeightMap;
	float m_angle;
	GLuint VertexArrayID;
	glm::mat4 perspect;
	glm::mat4 view;

	unsigned int texture;
	GLuint vertexbuffer;
	GLuint uvBuffer;

	GLuint color_texture;

	VertexArray* mVertexArray;
	Texture* mHeightMapTexture;
	Shader* shader;
};

