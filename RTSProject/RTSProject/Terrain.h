#pragma once
#include "Precompiled.h"
#include "HeightMap.h"
#include "Shader.h"
#include "VertexArrayAlpha.h"
#include "Texture.h"
#include "Model.h"
#include "Camera.h"

struct Patch 
{
	Patch();
	~Patch();

	void CreateMesh(HeightMap& hm, SDL_FRect source, int index);
	void Render(glm::mat4& mvpMat);
	void Update(float deltaTime);

	glm::mat4 perspect;
	glm::mat4 view;
	VertexArrayAlpha* mVertexArray;
	Shader* mMeshShader;
};

class Terrain
{
public:
	Terrain();
	~Terrain();

	void Initialize(glm::vec2 _size);
	void Release();
	void GenerateRandomTerrain(int numPatches);
	void CreatePatches(int numPatches);
	void CreateAlphaMaps();

	void Update(float deltaTime);
	void Render(Camera* camera);

	void AddObject(int type, glm::ivec2 p);

private:
	Texture* alpha;
	Texture* mountain;
	Texture* snow;
	Texture* grass;

	glm::vec2 mSize;
	HeightMap* mHeightMap;
	std::vector<Patch*> mPatches;
	std::vector<Model*> mModelList;
};

