#pragma once
#include "Precompiled.h"
#include "HeightMap.h"
#include "Shader.h"
#include "VertexArrayAlpha.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Camera.h"

struct Patch 
{
											Patch();
										   ~Patch();

	void									CreateMesh(HeightMap& hm, SDL_FRect source, int index);
	void									Render(glm::mat4& mvpMat);
	void									Update(float deltaTime);

	glm::mat4								perspect;
	glm::mat4								view;
	std::shared_ptr<VertexArrayAlpha>		mVertexArray;
	std::shared_ptr<Shader>					mMeshShader;
};

class Terrain
{
public:
											Terrain();
										   ~Terrain();

	void									Initialize(glm::vec2 _size);
	void									Release();
	void									GenerateRandomTerrain(int numPatches);
	void									CreatePatches(int numPatches);
	void									CreateAlphaMaps();

	void									Update(float deltaTime);
	void									Render(std::shared_ptr<Camera> camera);

	void									AddObject(int type, glm::ivec2 p);
	float									GetHeight(float x, float y);

private:
	std::shared_ptr<Texture>				alpha;
	std::shared_ptr<Texture>				mountain;
	std::shared_ptr<Texture>				snow;
	std::shared_ptr<Texture>				grass;

	glm::vec2								mSize;
	std::shared_ptr<HeightMap>				mHeightMap;
	std::vector<std::shared_ptr<Patch> >	mPatches;
	std::vector<std::shared_ptr<StaticMesh> >	mModelList;
};