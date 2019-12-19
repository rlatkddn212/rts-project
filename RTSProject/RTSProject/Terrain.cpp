#include "Precompiled.h"
#include "Terrain.h"
#include "Camera.h"

Patch::Patch()
{
	mVertexArray = nullptr;
}

Patch::~Patch()
{
}

void Patch::CreateMesh(HeightMap & hm, SDL_FRect source, int index)
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
			
			//Set make_shared<vertex
			if ((z0 * (width + 1) + x0)  > nrVert)
			{
				printf("over");
			}

			verts[(z0 * (width + 1) + x0) * 10] = pos.x;
			verts[(z0 * (width + 1) + x0) * 10 + 1] = pos.y;
			verts[(z0 * (width + 1) + x0) * 10 + 2] = pos.z;
			verts[(z0 * (width + 1) + x0) * 10 + 3] = normal.x;
			verts[(z0 * (width + 1) + x0) * 10 + 4] = normal.y;
			verts[(z0 * (width + 1) + x0) * 10 + 5] = normal.z;
			verts[(z0 * (width + 1) + x0) * 10 + 6] = auv.x * 8.0f;
			verts[(z0 * (width + 1) + x0) * 10 + 7] = auv.y * 8.0f;
			verts[(z0 * (width + 1) + x0) * 10 + 8] = auv.x;
			verts[(z0 * (width + 1) + x0) * 10 + 9] = auv.y;
		}
	}

	// ∆Æ∂Û¿Ãæﬁ±€ Ω∫∆Æ∏≥ æ∆¥‘.
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

void Patch::Render(glm::mat4& mvpMat)
{
	mVertexArray->SetActive();
	mMeshShader->SetActive();
	mMeshShader->SetMatrixUniform("mvp_matrix", mvpMat);
	
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

	for (int y = 0; y < mSize.y; y++)
	{
		for (int x = 0; x < mSize.x; x++)
		{
			if (mHeightMap->GetHeight(glm::ivec2(x, y)) == 0.0f && hm->GetHeight(glm::ivec2(x, y)) > 0.7f && rand() % 6 == 0)
				AddObject(0, glm::ivec2(x, y));	//Tree
			else if (mHeightMap->GetHeight(glm::ivec2(x, y)) >= 1.0f && hm->GetHeight(glm::ivec2(x, y)) > 0.9f && rand() % 100 == 0)
				AddObject(1, glm::ivec2(x, y));	//Stone
		}
	}
}

void Terrain::AddObject(int type, glm::ivec2 p)
{
	glm::vec3 pos = glm::vec3((float)p.x, mHeightMap->GetHeight(p), (float)-p.y);
	glm::vec3 rot = glm::vec3((rand() % 1000 / 1000.0f) * 3.0f, (rand() % 1000 / 1000.0f) * 0.13f, (rand() % 1000 / 1000.0f) * 0.13f);

	float sca_xz = (rand() % 1000 / 1000.0f) * 0.5f + 0.5f;
	float sca_y = (rand() % 1000 / 1000.0f) * 1.0f + 0.5f;
	glm::vec3 sca = glm::vec3(sca_xz, sca_y, sca_xz);

	std::shared_ptr<Model> obj = std::make_shared<Model>();

	if (type == 0)
	{
		obj->LoadModel("tree.x");
	}
	else
	{
		obj->LoadModel("stone.x");
	}

	obj->SetPosition(pos);
	obj->SetRotation(rot);
	obj->SetScale(sca);
	mModelList.push_back(obj);
}

void Terrain::CreatePatches(int numPatches)
{
	mPatches.clear();

	if (mHeightMap == NULL)return;

	//Create make_shared<patches
	for (int y = 0; y < numPatches; y++)
	{
		for (int x = 0; x < numPatches; x++)
		{
			SDL_FRect r = { (int)(x * (mSize.x - 1) / (float)numPatches),
					  (int)(y * (mSize.y - 1) / (float)numPatches),
					(int)((x + 1) * (mSize.x - 1) / (float)numPatches) - (int)(x * (mSize.x - 1) / (float)numPatches),
					(int)((y + 1) * (mSize.y - 1) / (float)numPatches) - (int)(y * (mSize.y - 1) / (float)numPatches) };

			std::shared_ptr<Patch> p = std::make_shared<Patch>();
			p->CreateMesh(*mHeightMap, r, x + y);
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

	for (int i = 0; i < 3; i++)
	{
		for (int y = 0; y < 128; y++)
		{
			for (int x = 0; x < 128; x++)
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
	alpha->LoadRawData(bytes, 128, 128, 4);

	delete[] bytes;
}

void Terrain::Update(float deltaTime)
{
	for (int i = 0; i < (int)mPatches.size(); i++)
		mPatches[i]->Update(deltaTime);
}

void Terrain::Render(std::shared_ptr<Camera> camera)
{
	float aspect = (float)1024 / (float)768;
	
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

	for (int i = 0; i < (int)mPatches.size(); i++)
		mPatches[i]->Render(mat);

	for (int i = 0; i < mModelList.size(); ++i)
	{
		mModelList[i]->SetPerspect(perspect);
		mModelList[i]->SetView(view);
		mModelList[i]->RenderModel(camera);
	}
}