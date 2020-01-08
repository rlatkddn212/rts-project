#pragma once
#include "Precompiled.h"
#include "HeightMap.h"
#include "Shader.h"
#include "VertexArrayAlpha.h"
#include "Texture.h"
#include "MapObject.h"
#include "Camera.h"
#include "Ray.h"

class pqComp
{
public:
	
	pqComp(const bool& revparam = true)
	{
		reverse = revparam;
	}

	bool operator() (const std::pair<float, glm::ivec2>& lhs, const std::pair<float, glm::ivec2>& rhs) const
	{
		if (reverse) return (lhs.first > rhs.first);
		else return (lhs.first < rhs.first);
	}
	
private:
	bool reverse;
};


enum TileState
{
	None,
	StaticObject,
	MoveObject,
	StopObject,
};

struct Tile
{
	glm::ivec2 xy;
	bool isMovable;
	float cost;
	bool isOpen;
	bool isClose;
	float f;
	float g;

	TileState mTileState;
	
	Tile* parent;
	Tile* neigbors[8];

	Tile()
	{
		mTileState = None;
		
		cost = 0.0f;
		isMovable = true;
		isOpen = false;
		isClose = false;
		f = 0.0f;
		g = 0.0f;
		parent = nullptr;
		for (int i = 0; i < 8; ++i)
		{
			neigbors[i] = nullptr;
		}
	}
};

struct Patch 
{
											Patch();
										   ~Patch();

	void								    SetRect(SDL_FRect source) { mRect = source;}
	SDL_FRect								GetRect() { return mRect; }

	void									CreateMesh(HeightMap& hm, SDL_FRect source);
	void									Render(glm::mat4& vpMat);
	void									Update(float deltaTime);
	
	SDL_FRect								mRect;
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
	void									Update(float deltaTime);
	void									Render(std::shared_ptr<Camera> camera);

	void									GenerateRandomTerrain(int numPatches);
	void									CreatePatches(int numPatches);
	void									CreateAlphaMaps();

	void									AddObject(int type, glm::ivec2 p);
	void									CreatePath();

	std::vector<glm::ivec2>					GetPath(glm::ivec2 startPos, glm::ivec2 endPos, bool isObjectCollision = true);
	float									GetHeight(glm::vec2 p);
	glm::vec2								GetSize() { return mSize; }

	bool									Intersect(Ray& ray, glm::ivec2& ret);
	bool									RayTriangleIntersect(
												const glm::vec3& orig, const glm::vec3& dir,
												const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, glm::vec3& P);
	
	bool									GetClosedPosition(glm::ivec2 p1, glm::ivec2* closePos);

	void									InitTileState();
	void									SetTileState(glm::ivec2 p, TileState type) { if (mTile[p.y][p.x].mTileState != StaticObject) mTile[p.y][p.x].mTileState = type; }

	Tile*									GetTile(glm::ivec2 p) { return &(mTile[p.y][p.x]); }
	TileState								GetTileState(glm::ivec2 p) { return mTile[p.y][p.x].mTileState; };
	bool									IsObjectOnTile(glm::ivec2 p) { return (mTile[p.y][p.x].mTileState != TileState::None) ? true : false; };

	bool									IsMovableTile(glm::ivec2 p) { return mTile[p.y][p.x].isMovable; };

	void									SetFogTexture(std::shared_ptr<Texture> texture) { mFogTexture = texture; }

	void									CreateMapTexture();
	std::shared_ptr<Texture>				GetMapTexture() { return mMapTexture; }
	void									PrintScreen(GLuint framebuffer, const std::string& str);

private:

	// 배열 크기는 고정, 조절 필요
	Tile									mTile[100][100];
	std::vector<std::pair<int, int> >		mObjectPos;
	std::shared_ptr<Texture>				alpha;
	std::shared_ptr<Texture>				mountain;
	std::shared_ptr<Texture>				snow;
	std::shared_ptr<Texture>				grass;

	std::shared_ptr<Texture>				mFogTexture;

	glm::vec2								mSize;
	std::shared_ptr<HeightMap>				mHeightMap;
	std::vector<std::shared_ptr<Patch> >	mPatches;
	std::vector<std::shared_ptr<MapObject> > mModelList;

	std::shared_ptr<Texture>				mMapTexture;
};