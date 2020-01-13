#include "Precompiled.h"
#include "EffectResourcePool.h"
using namespace std;

void EffectResourcePool::Initialize()
{
	float vertices[] =
	{
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
		0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	mVerts = std::make_shared<VertexArray>(vertices, 4, indices, 6);
	std::vector<std::pair<std::string, int> > shaderCodies;

	mShader = std::make_shared<Shader>();
	shaderCodies.push_back(make_pair(ReadShaderFile("Sprite.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("Sprite.frag"), GL_FRAGMENT_SHADER));
	mShader->BuildShader(shaderCodies);

	Load();
}

void EffectResourcePool::Load()
{
	mCloudTexture = make_shared<Texture>();
	mFireballTexture = make_shared<Texture>();
	mFireTexture = make_shared<Texture>();
	mNoiseTexture = make_shared<Texture>();
	mRunesTexture = make_shared<Texture>();
	// load Texture
	mCloudTexture->Load("Assets/Model/cloud.dds");
	mFireballTexture->Load("Assets/Model/fireball.dds");
	mFireTexture->Load("Assets/Model/fire.dds");
	mNoiseTexture->Load("Assets/Model/noise.dds");
	mRunesTexture->Load("Assets/Model/runes.dds");
}
