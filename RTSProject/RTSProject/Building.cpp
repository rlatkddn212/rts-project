#include "Precompiled.h"
#include "Building.h"


Building::Building(int type)
{
	mType = type;
	if (type == 0)
	{
		LoadModel("Assets/Model/barracks.X");
	}
	else if (type == 1)
	{
		LoadModel("Assets/Model/tower.x");
	}
	else if (type == 2)
	{
		LoadModel("Assets/Model/townhall.x");
	}

	SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
}

Building::~Building()
{
}

void Building::LoadShader()
{
	std::vector<std::pair<std::string, int> > shaderCodies;
	shaderCodies.push_back(make_pair(ReadShaderFile("building.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("building.frag"), GL_FRAGMENT_SHADER));
	mMeshShader = std::make_shared<Shader>();
	mMeshShader->BuildShader(shaderCodies);
}
