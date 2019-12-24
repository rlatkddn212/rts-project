#include "Precompiled.h"
#include "Building.h"

using namespace std;

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

	SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
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

bool Building::isPossibleBuild(shared_ptr<Terrain> terrain, int x, int y)
{
	glm::vec2 size = terrain->GetSize();
	// 건물마다 다르겠지만 우선 3x3 칸을 사용
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			int nx = x + i - 1;
			int ny = y + j - 1;

			bool isBuild = false;

			if (0 <= nx && nx < size.x && 0 <= ny && ny < size.y)
			{
				if (terrain->IsMovableTile(nx, ny)
					&& !terrain->IsObjectOnTile(nx, ny)
					&& !terrain->IsUnitOnTile(nx, ny))
				{
					isBuild = true;
				}
			}
			
			if (!isBuild)
			{
				return false;
			}
		}
	}

	return true;
}

void Building::BuildOnTerrain(std::shared_ptr<Terrain> terrain, int x, int y)
{
	SetPosition(glm::vec3(x, terrain->GetHeight(x, -y), -y));

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			int nx = x + i - 1;
			int ny = y + j - 1;

			terrain->SetObjectOnTile(x, y);
		}
	}
}

void Building::SetUniform()
{
	mMeshShader->SetVectorUniform("buildingColor", mColor);
}
