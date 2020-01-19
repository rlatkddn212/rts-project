#include "Precompiled.h"
#include "Terrain.h"
#include "Camera.h"
#include <queue>
#include "Math3D.h"

using namespace std;

Patch::Patch()
{
	mVertexArray = nullptr;
}

Patch::~Patch()
{
}

void Patch::CreateMesh(HeightMap & hm, SDL_FRect source)
{
	int width = source.w;
	int height = source.h;
	int nrVert = (width + 1) * (height + 1);
	int nrTri = width * height * 2;

	float* verts = new float[10 * nrVert];
	unsigned int numVerts = nrVert;

	unsigned int* indices = new unsigned int[6 * nrVert];
	unsigned int numIndices = 0;

	for (int z = source.y, z0 = 0; z <= source.y + source.h; z++, z0++)
	{
		for (int x = source.x, x0 = 0; x <= source.x + source.w; x++, x0++)
		{
			//Calculate vertex color
			float prc = hm.mHeightMap[x + z * hm.mSize.x] / hm.mMaxHeight;
			int red = (int)(255 * prc);
			int green = (int)(255 * (1.0f - prc));

			glm::vec3 pos = glm::vec3((float)x, hm.mHeightMap[x + z * hm.mSize.x], (float)-z);
			glm::vec2 auv = glm::vec2(x / (float)hm.mSize.x, z / (float)hm.mSize.y);
			glm::vec3 normal = glm::vec3(0.0, 1.0, 0.0);
			
			if ((z0 * (width + 1) + x0)  > nrVert)
			{
				printf("over");
			}

			// 버텍스
			verts[(z0 * (width + 1) + x0) * 10] = pos.x;
			verts[(z0 * (width + 1) + x0) * 10 + 1] = pos.y;
			verts[(z0 * (width + 1) + x0) * 10 + 2] = pos.z;
			// 노멀
			verts[(z0 * (width + 1) + x0) * 10 + 3] = normal.x;
			verts[(z0 * (width + 1) + x0) * 10 + 4] = normal.y;
			verts[(z0 * (width + 1) + x0) * 10 + 5] = normal.z;
			// 텍스쳐
			verts[(z0 * (width + 1) + x0) * 10 + 6] = auv.x * 8.0f;
			verts[(z0 * (width + 1) + x0) * 10 + 7] = auv.y * 8.0f;
			// 알파
			verts[(z0 * (width + 1) + x0) * 10 + 8] = auv.x;
			verts[(z0 * (width + 1) + x0) * 10 + 9] = auv.y;
		}
	}

	// 트라이앵글 스트립 아님.
	for (int z = source.y, z0 = 0; z < source.y + source.h; z++, z0++)
	{
		for (int x = source.x, x0 = 0; x < source.x + source.w; x++, x0++)
		{
			if (numIndices > nrVert * 6 )
			{
				printf("over");
			}

			//Triangle 1
			indices[numIndices++] = z0 * (width + 1) + x0;
			indices[numIndices++] = z0 * (width + 1) + x0 + 1;
			indices[numIndices++] = (z0 + 1) * (width + 1) + x0;

			//Triangle 2
			indices[numIndices++] = (z0 + 1) * (width + 1) + x0;
			indices[numIndices++] = z0 * (width + 1) + x0 + 1;
			indices[numIndices++] = (z0 + 1) * (width + 1) + x0 + 1;
		}
	}

	mVertexArray = std::make_shared<VertexArrayAlpha>(verts, numVerts, indices, numIndices);

	std::vector<std::pair<std::string, int> > shaderCodies;
	shaderCodies.push_back(make_pair(ReadShaderFile("terrain.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("terrain.frag"), GL_FRAGMENT_SHADER));
	mMeshShader = std::make_shared<Shader>();
	mMeshShader->BuildShader(shaderCodies);

	delete[] verts;
	delete[] indices;
}

void Patch::Render(glm::mat4& vpMat)
{
	mVertexArray->SetActive();
	mMeshShader->SetActive();
	mMeshShader->SetMatrixUniform("vpMatrix", vpMat);
	
	glDrawElements(GL_TRIANGLES, mVertexArray->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}

void Patch::Update(float deltaTime)
{
}

Terrain::Terrain()
{
	mountain = std::make_shared<Texture>();
	snow = std::make_shared<Texture>();
	grass = std::make_shared<Texture>();

	grass->Load("Assets/grass.jpg");
	mountain->Load("Assets/mountain.jpg");
	snow->Load("Assets/snow.jpg");
}

Terrain::~Terrain()
{
}

void Terrain::Initialize(glm::vec2 size)
{
	mSize = size;
	mHeightMap = nullptr;
	InitTileState();
	GenerateRandomTerrain(3);
}

void Terrain::Release()
{
}

void Terrain::GenerateRandomTerrain(int numPatches)
{
	mHeightMap = std::make_shared<HeightMap>(mSize);
	mHeightMap->CreateRandomHeightMap(rand() % 2000, 2.5f, 0.5f, 8);
	//mHeightMap->LoadFromFile("smiley.bmp");
	CreatePatches(numPatches);
	CreateAlphaMaps();

	std::shared_ptr<HeightMap> hm = std::make_shared<HeightMap>(mSize);
	
	hm->CreateRandomHeightMap(rand() % 2000, 5.5f, 0.9f, 7);

	for (int y = 0; y < mSize.y; ++y)
	{
		for (int x = 0; x < mSize.x; ++x)
		{
			if (mHeightMap->GetHeight(glm::ivec2(x, y)) == 0.0f && hm->GetHeight(glm::ivec2(x, y)) > 0.7f && rand() % 6 == 0)
				AddObject(0, glm::ivec2(x, y));	//Tree
			else if (mHeightMap->GetHeight(glm::ivec2(x, y)) >= 1.0f && hm->GetHeight(glm::ivec2(x, y)) > 0.9f && rand() % 100 == 0)
				AddObject(1, glm::ivec2(x, y));	//Stone
		}
	}

	CreatePath();
	
	mMapTexture = make_shared<Texture>();
	mMapTexture->CreateTexture(1024, 768, 0);
	CreateMapTexture();
}

void Terrain::AddObject(int type, glm::ivec2 p)
{
	SetTileState(p, TileState::StaticObject);

	glm::vec3 pos = glm::vec3((float)p.x, mHeightMap->GetHeight(p), (float)-p.y);
	glm::vec3 rot = glm::vec3((rand() % 1000 / 1000.0f) * 3.0f, (rand() % 1000 / 1000.0f) * 0.13f, (rand() % 1000 / 1000.0f) * 0.13f);

	float sca_xz = (rand() % 1000 / 1000.0f) * 0.5f + 0.5f;
	float sca_y = (rand() % 1000 / 1000.0f) * 1.0f + 0.5f;
	glm::vec3 sca = glm::vec3(sca_xz, sca_y, sca_xz);

	std::shared_ptr<MapObject> obj = std::make_shared<MapObject>(type);

	obj->SetPosition(pos);
	obj->SetRotation(rot);
	obj->SetScale(sca);
	mModelList.push_back(obj);
}


float cross(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2)
{
	float temp = p0.x * p1.y + p1.x * p2.y + p2.x * p0.y;
	temp = temp - p0.y * p1.x - p1.y * p2.x - p2.y * p0.x;
	if (temp >= 0.0f)
	{
		return temp;
	}
	else
	{
		return temp;
	}
}

float Terrain::GetHeight(glm::vec2 p)
{
	
	glm::vec2 pos = p;
	for (int y = 0; y < mSize.y - 1; ++y)
	{
		for (int x = 0; x < mSize.x - 1; ++x)
		{
			{
				glm::vec2 p0 = glm::vec2(x, -y);
				glm::vec2 p1 = glm::vec2(x, -y - 1);
				glm::vec2 p2 = glm::vec2(x + 1, -y);
				
				if (cross(p0, p1, pos) >= 0.0f && cross(p1, p2, pos) >= 0.0f&& cross(p2, p0, pos) >= 0.0f)
				{
					return (mHeightMap->GetHeight(glm::ivec2(p2.x, -p2.y)) * cross(p0, p1, pos)
					+ mHeightMap->GetHeight(glm::ivec2(p0.x, -p0.y)) * cross(p1, p2, pos)
					+ mHeightMap->GetHeight(glm::ivec2(p1.x, -p1.y)) * cross(p2, p0, pos)) / cross(p0, p1, p2);
				}
			}

			{
				glm::vec2 p0 = glm::vec2(x + 1, -y - 1);
				glm::vec2 p1 = glm::vec2(x + 1, -y);
				glm::vec2 p2 = glm::vec2(x, -y - 1);

				if (cross(p0, p1, pos) >= 0.0f && cross(p1, p2, pos) >= 0.0f&& cross(p2, p0, pos) >= 0.0f)
				{
					return (mHeightMap->GetHeight(glm::ivec2(p2.x, -p2.y)) * cross(p0, p1, pos)
						+ mHeightMap->GetHeight(glm::ivec2(p0.x, -p0.y)) * cross(p1, p2, pos)
						+ mHeightMap->GetHeight(glm::ivec2(p1.x, -p1.y)) * cross(p2, p0, pos)) / cross(p0, p1, p2);
				}
			}
		}
	}

	return mHeightMap->GetHeight(RoundPosition(glm::vec2(p.x, -p.y)));
}

void Terrain::CreatePath()
{
	int dy[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
	int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
	// 패스 생성
	for (int y = 0; y < mSize.y; ++y)
	{
		for (int x = 0; x < mSize.x; ++x)
		{
			float height = mHeightMap->GetHeight(glm::vec2(x, y));
			for (int k = 0; k < 8; ++k)
			{
				int neighborX = x + dx[k];
				int neighborY = y + dy[k];
				
				if (0 <= neighborX && neighborX < mSize.x 
					&& 0 <= neighborY && neighborY < mSize.y)
				{
					Tile& neighborTile = mTile[neighborY][neighborX];
					mTile[y][x].neigbors[k] = &neighborTile;
					
					neighborTile.xy = glm::ivec2(neighborX, neighborY);
					
					float neighborHeight = mHeightMap->GetHeight(glm::vec2(neighborX, neighborY));
					neighborTile.cost = abs(height - neighborHeight);
					if (neighborTile.cost > 1.0f)
					{
						neighborTile.isMovable = false;
					}
				}
			}
		}
	}
}

std::vector<glm::ivec2> Terrain::GetPath(glm::ivec2 startPos, glm::ivec2 endPos, bool isObjectCollision)
{
	assert(startPos.y >= 0);
	assert(endPos.y >= 0);

	std::vector<glm::ivec2> ret;

	priority_queue<pair<float, glm::ivec2>, vector<pair<float, glm::ivec2>>, pqComp > pq;
	pq.push(make_pair(0.0f, startPos));
	
	// 초기화
	for (int y = 0; y < mSize.y; ++y)
	{
		for (int x = 0; x < mSize.x; ++x)
		{
			mTile[y][x].f = mTile[y][x].g = 0.0f;
			mTile[y][x].parent = nullptr;
			mTile[y][x].isClose = mTile[y][x].isOpen = false;
		}
	}
	
	mTile[startPos.y][startPos.x].isOpen = true;

	while (! pq.empty())
	{
		float cost = pq.top().first;
		glm::ivec2 pos = pq.top().second;
		pq.pop();

		Tile& nowTile = mTile[pos.y][pos.x];
		if (nowTile.isClose) continue;
		nowTile.isClose = true;
		
		// 도착한 경우
		if (nowTile.xy == endPos)
		{
			Tile* point = &nowTile;
			
			while (point != nullptr)
			{
				ret.push_back(point->xy);
				point = point->parent;
			}
			ret.pop_back();
			reverse(ret.begin(), ret.end());

			break;
		}

		for (int i = 0; i < 8; ++i)
		{
			if (nowTile.neigbors[i] != nullptr)
			{
				Tile* neigborTile = nowTile.neigbors[i];
				if (neigborTile->isMovable)
				{
					if (isObjectCollision && neigborTile->mTileState != TileState::None)
					{
						continue;
					}

					float newG = nowTile.g + glm::distance(glm::vec2(pos.x, pos.y), glm::vec2(neigborTile->xy.x, neigborTile->xy.y));
					float newF = newG + glm::distance(glm::vec2(endPos.x, endPos.y), glm::vec2(neigborTile->xy.x, neigborTile->xy.y));
					
					// 닫힌 경우
					if (neigborTile->isClose)
					{
						// 기존 것보다 작다면 대체 한다.
						if (newF < neigborTile->f)
						{
							neigborTile->g = newG;
							neigborTile->f = newF;
							neigborTile->parent = &nowTile;
						}
					}
					else if (neigborTile->isOpen)
					{
						if (newF < neigborTile->f)
						{
							neigborTile->g = newG;
							neigborTile->f = newF;
							neigborTile->parent = &nowTile;
							
							// 맵이 클 경우 문제가 될 수 있음 pq size를 고려해서 최적화 시킬 필요가 있음
							pq.push(make_pair(newF, neigborTile->xy));
						}
					}
					else
					{
						neigborTile->g = newG;
						neigborTile->f = newF;
						neigborTile->parent = &nowTile;
						neigborTile->isOpen = true;

						pq.push(make_pair(newF, neigborTile->xy));
					}
				}
			}
		}
	}

	return ret;
}

bool Terrain::Intersect(Ray& ray, glm::ivec2& ret)
{
	glm::vec3 pos = ray.org;
	glm::vec3 dir = ray.dir;
	dir = glm::normalize(dir);

	for (int y = 0; y < mSize.y - 1; ++y)
	{
		for (int x = 0; x < mSize.x - 1; ++x)
		{
			glm::vec3 p;
			if (RayTriangleIntersect(ray.org, dir,
				glm::vec3(x, mHeightMap->GetHeight(glm::ivec2(x, y)), -y),
				glm::vec3(glm::vec3(x, mHeightMap->GetHeight(glm::ivec2(x, y + 1)), -y - 1)),
				glm::vec3(glm::vec3(x + 1, mHeightMap->GetHeight(glm::ivec2(x + 1, y)), -y)),
				p))
			{
				p.z *= -1;
				int nx = (int)p.x + (((p.x - (int)p.x) < 0.5f) ? 0 : 1);
				int ny = (int)p.z + (((p.z - (int)p.z) < 0.5f) ? 0 : 1);
				ret = glm::ivec2(nx, ny);
				
				return true;
			}

			if (RayTriangleIntersect(ray.org, dir,
				glm::vec3(x + 1, mHeightMap->GetHeight(glm::ivec2(x + 1, y + 1)), -y - 1),
				glm::vec3(glm::vec3(x + 1, mHeightMap->GetHeight(glm::ivec2(x, y)), -y)),
				glm::vec3(glm::vec3(x, mHeightMap->GetHeight(glm::ivec2(x, y + 1)), -y - 1)),
				p))
			{
				p.z *= -1;
				int nx = (int)p.x + (((p.x - (int)p.x) < 0.5f) ? 0 : 1);
				int ny = (int)p.z + (((p.z - (int)p.z) < 0.5f) ? 0 : 1);
				ret = glm::ivec2(nx, ny);
				
				return true;
			}
		}
	}
	
	return false;
}

bool Terrain::RayTriangleIntersect(const glm::vec3 & orig, const glm::vec3& dir, 
		const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, glm::vec3& P)
{
	glm::vec3 v0v1 = v1 - v0;
	glm::vec3 v0v2 = v2 - v0;
	glm::vec3 N = glm::cross(v0v1, v0v2);
	float area2 = N.length();

	float NdotRayDirection = glm::dot(N, dir);
	if (fabs(NdotRayDirection) < 0.00001)
	{
		return false;
	}

	float t = glm::dot((v0 - orig), N) / NdotRayDirection;
	if (t < 0) return false;

	P = orig + t * dir;

	glm::vec3 C;

	glm::vec3 edge0 = v1 - v0;
	glm::vec3 vp0 = P - v0;

	C = glm::cross(edge0, vp0);
	if (glm::dot(N, C) < 0) return false;

	glm::vec3 edge1 = v2 - v1;
	glm::vec3 vp1 = P - v1;
	C = glm::cross(edge1, vp1);
	if (glm::dot(N, C) < 0)  return false;

	glm::vec3 edge2 = v0 - v2;
	glm::vec3 vp2 = P - v2;
	C = glm::cross(edge2, vp2);
	if (glm::dot(N, C) < 0) return false;

	return true;
}

bool Terrain::GetClosedPosition(glm::ivec2 p1, glm::ivec2* closePos)
{
	int n = 1;
	int nx = p1.x;
	int ny = p1.y;
	int d = 1;
	
	while (n < 15)
	{
		if (0 <= ny && ny < mSize.y)
		{
			for (int i = 0; i < n; ++i)
			{
				nx = nx + d;
				if (0 <= nx && nx < mSize.x)
				{
					glm::ivec2 newPos = glm::ivec2(nx, ny);
					if (IsMovableTile(newPos) && !IsObjectOnTile(newPos))
					{
						*closePos = newPos;
						
						return true;
					}
				}
			}
		}
		if (0 <= nx && nx < mSize.x)
		{
			for (int i = 0; i < n; ++i)
			{
				ny = ny + d;
				if (0 <= ny && ny < mSize.y)
				{
					glm::ivec2 newPos = glm::ivec2(nx, ny);
					if (IsMovableTile(newPos) && !IsObjectOnTile(newPos))
					{
						*closePos = newPos;
		
						return true;
					}
				}
			}
		}
		
		n++;
		d *= -1;
	}

	return false;
}

void Terrain::InitTileState()
{
	for (int i = 0; i < 100; ++i)
	{
		for (int j = 0; j < 100; ++j)
		{
			mTile[i][j].mTileState = TileState::None;
		}
	}
}

void Terrain::SetTileState(glm::ivec2 p, TileState type)
{
	assert(p.y >= 0);
	if (mTile[p.y][p.x].mTileState != StaticObject) 
		mTile[p.y][p.x].mTileState = type;
}

Tile * Terrain::GetTile(glm::ivec2 p)
{
	assert(p.y >= 0);
	return &(mTile[p.y][p.x]);
}

TileState Terrain::GetTileState(glm::ivec2 p)
{
	assert(p.y >= 0);
	return mTile[p.y][p.x].mTileState;
}

bool Terrain::IsObjectOnTile(glm::ivec2 p)
{
	assert(p.y >= 0);
	return (mTile[p.y][p.x].mTileState != TileState::None) ? true : false;
}

bool Terrain::IsMovableTile(glm::ivec2 p)
{
	assert(p.y >= 0);
	return mTile[p.y][p.x].isMovable;
}

void Terrain::SetFogTexture(std::shared_ptr<Texture> texture)
{
	mFogTexture = texture;
}

void Terrain::CreatePatches(int numPatches)
{
	mPatches.clear();

	if (mHeightMap == NULL)return;

	//Create make_shared<patches
	for (int y = 0; y < numPatches; ++y)
	{
		for (int x = 0; x < numPatches; ++x)
		{
			SDL_FRect r = { (int)(x * (mSize.x - 1) / (float)numPatches),
					  (int)(y * (mSize.y - 1) / (float)numPatches),
					(int)((x + 1) * (mSize.x - 1) / (float)numPatches) - (int)(x * (mSize.x - 1) / (float)numPatches),
					(int)((y + 1) * (mSize.y - 1) / (float)numPatches) - (int)(y * (mSize.y - 1) / (float)numPatches) };

			std::shared_ptr<Patch> p = std::make_shared<Patch>();
			p->CreateMesh(*mHeightMap, r);
			mPatches.push_back(p);
		}
	}
}

void Terrain::CreateAlphaMaps()
{
	unsigned char* bytes = new unsigned char[128 * 128 * 4];
	memset(bytes, 0, 128 * 128 * 4);
	float min_range[] = { 0.0f, 1.0f, 15.0f };
	float max_range[] = { 2.0f, 16.0f, 21.0f };

	for (int i = 0; i < 3; ++i)
	{
		for (int y = 0; y < 128; ++y)
		{
			for (int x = 0; x < 128; ++x)
			{
				int hm_x = (int)(mHeightMap->mSize.x * (x / (float)(128)));
				int hm_y = (int)(mHeightMap->mSize.y * (y / (float)(128)));
				float height = mHeightMap->mHeightMap[hm_x + hm_y * mHeightMap->mSize.x];

				unsigned char *b = bytes + y * 128 * 4 + x * 4 + i;
				if (height >= min_range[i] && height <= max_range[i])
				{
					*b = 255;
				}
				else
				{
					*b = 0;
				}
			}
		}
	}

	alpha = std::make_shared<Texture>();
	alpha->CreateFromRawData(bytes, 128, 128, 4);

	delete[] bytes;
}

void Terrain::Update(float deltaTime)
{
	for (int i = 0; i < (int)mPatches.size(); ++i)
		mPatches[i]->Update(deltaTime);
}

void Terrain::Render(std::shared_ptr<Camera> camera)
{
	glm::mat4 mat = glm::mat4(1.0f);
	glm::mat4 perspect = camera->GetProjectionMatrix();
	glm::mat4 view = camera->GetViewMatrix();
	mat = perspect * view * mat;
	
	glActiveTexture(GL_TEXTURE0);
	grass->SetActive();
	glActiveTexture(GL_TEXTURE1);
	mountain->SetActive();
	glActiveTexture(GL_TEXTURE2);
	snow->SetActive();
	glActiveTexture(GL_TEXTURE3);
	alpha->SetActive();
	glActiveTexture(GL_TEXTURE4);
	mFogTexture->SetActive();

	for (int i = 0; i < (int)mPatches.size(); ++i)
	{
		mPatches[i]->Render(mat);
	}

	glActiveTexture(GL_TEXTURE1);
	mFogTexture->SetActive();
	for (int i = 0; i < mModelList.size(); ++i)
	{
		mModelList[i]->Render(camera);
	}
	glActiveTexture(GL_TEXTURE0);
}

void Terrain::AddRender(std::shared_ptr<Camera> camera)
{
	std::shared_ptr<Terrain> ro = std::make_shared<Terrain>(*this);
	ro->mCamera = camera;
	RenderManager::GetInstance()->AddQueue(ro);
}

std::shared_ptr<Texture> Terrain::GetMapTexture()
{
	return mMapTexture;
}

void Terrain::CreateMapTexture()
{
	glm::mat4 proj = glm::ortho(-49.5f, 49.5f, -49.5f, 49.5f, -1000.0f, 1000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(49.5f, 100.0f, -49.5f), glm::vec3(49.5f, 0.0f, -49.5f), glm::vec3(0.0f, 0.0f, -1.0f));

	glm::mat4 mat = proj * view;

	glm::vec4 pos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 newp = mat * pos;

	// frame Buffer 생성
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mMapTexture->GetTextureID(), 0);

	// Frame buffer 완전성 위배
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return;
	}

	static const GLfloat black[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, black);

	glActiveTexture(GL_TEXTURE0);
	grass->SetActive();
	glActiveTexture(GL_TEXTURE1);
	mountain->SetActive();
	glActiveTexture(GL_TEXTURE2);
	snow->SetActive();
	glActiveTexture(GL_TEXTURE3);
	alpha->SetActive();
	glActiveTexture(GL_TEXTURE4);

	shared_ptr<Texture> voidTexture = make_shared<Texture>();
	voidTexture->CreateTexture(100, 100, 255);
	voidTexture->SetActive();
	
	for (int i = 0; i < (int)mPatches.size(); ++i)
	{
		mPatches[i]->Render(mat);
	}

	glActiveTexture(GL_TEXTURE0);
	PrintScreen(frameBuffer, "Maptexture.bmp");
	glDeleteFramebuffers(1, &frameBuffer);
}

void Terrain::PrintScreen(GLuint framebuffer, const std::string& str)
{
	unsigned char* bytes = new unsigned char[1024 * 1024 * 3];
	GLuint drawFrameBuffer;
	glGenFramebuffers(1, &drawFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, drawFrameBuffer);

	// create a color attachment texture
	std::shared_ptr<Texture> textureColorbuffer2 = std::make_shared<Texture>();
	textureColorbuffer2->CreateTexture(1024, 1024, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer2->GetTextureID(), 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("frame buffer 완전성 위배");
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFrameBuffer);
	glBlitFramebuffer(0, 0, 1024, 768, 0, 0, 1024, 1024, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, drawFrameBuffer);
	glReadPixels(0, 0, 1024, 1024, GL_RGB, GL_UNSIGNED_BYTE, bytes);

	SOIL_save_image(str.c_str(), SOIL_SAVE_TYPE_BMP, 1024, 1024, 3, bytes);

	delete[] bytes;
	glDeleteFramebuffers(1, &drawFrameBuffer);
}
