#include "Precompiled.h"
#include "BuildingMesh.h"

void BuildingMesh::LoadShader()
{
	std::vector<std::pair<std::string, int> > shaderCodies;
	shaderCodies.push_back(make_pair(ReadShaderFile("building.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("building.frag"), GL_FRAGMENT_SHADER));
	mMeshShader = std::make_shared<Shader>();
	mMeshShader->BuildShader(shaderCodies);
}

void BuildingMesh::SetUniform()
{
	mMeshShader->SetVector4Uniform("teamColor", mColor);
}
