#include "Precompiled.h"
#include "Model.h"

Model::Model()
{
	m_angle = 0.0f;
	SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
}

Model::~Model()
{
}

void Model::LoadModel(const std::string & fileName)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		printf("Model (%s) failed to load: %s", fileName, importer.GetErrorString());
		return;
	}

	aiMatrix4x4 mat;
	LoadNode(scene->mRootNode, scene, mat);
	LoadMaterials(scene);

	std::vector<std::pair<std::string, int> > shaderCodies;
	shaderCodies.push_back(make_pair(ReadShaderFile("mesh.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("mesh.frag"), GL_FRAGMENT_SHADER));
	mMeshShader = new Shader();
	mMeshShader->BuildShader(shaderCodies);
}

void Model::Update(float deltaTime)
{
	m_angle += deltaTime * 0.5f;

	float aspect = (float)1024 / (float)768;
	perspect = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 1000.0f);
	view = glm::lookAt(glm::vec3(cos(m_angle) * 2.0f, 1.5f, sin(m_angle) * 2.0f),
		glm::vec3(0.0f, 0.5f, 0.0f),
		glm::vec3(0.0, 1.0, 0.0));
}

void Model::RenderModel(Camera* camera)
{
	glm::mat4 mat = camera->GetProjectionMatrix() * camera->GetViewMatrix() * mPos * mRot * mSca;
	mMeshShader->SetMatrixUniform("mvp_matrix", mat);
	mMeshShader->SetActive();

	for (size_t i = 0; i < meshList.size(); i++)
	{
		unsigned int materialIndex = meshToTex[i];
		if (materialIndex < textureList.size() && textureList[materialIndex])
		{
			glActiveTexture(GL_TEXTURE0);
			textureList[materialIndex]->SetActive();
		}

		meshList[i]->SetActive();
		glDrawElements(GL_TRIANGLES, meshList[i]->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void Model::ClearModel()
{
}

void Model::LoadNode(aiNode * node, const aiScene * scene, aiMatrix4x4 mat)
{
	aiMatrix4x4 localMat = mat * node->mTransformation;
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		LoadMesh(localMat, scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene, localMat);
	}
}

void Model::LoadMesh(aiMatrix4x4 mat, aiMesh * mesh, const aiScene * scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		mesh->mVertices[i] = mat * mesh->mVertices[i];
		mesh->mNormals[i] = mat * mesh->mNormals[i];
		// 정점
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		// 노멀
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
		// 텍스쳐 좌표
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else {
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	
	VertexArray* vertex = new VertexArray(&vertices[0], mesh->mNumVertices, &indices[0], indices.size());
	meshList.push_back(vertex);
	meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene * scene)
{
	textureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];
		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);

				std::string texPath = filename;

				textureList[i] = new Texture();
				textureList[i]->Load(texPath.c_str());
			}
		}
	}
}
