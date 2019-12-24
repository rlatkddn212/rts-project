#include "Precompiled.h"
#include "StaticMesh.h"

StaticMesh::StaticMesh()
{
	mAngle = 0.0f;
	SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
}

StaticMesh::~StaticMesh()
{
}

void StaticMesh::LoadModel(const std::string & fileName)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(fileName, 
		aiProcess_Triangulate |
		aiProcess_FlipUVs | 
		aiProcess_GenSmoothNormals | 
		aiProcess_JoinIdenticalVertices
	);

	if (!scene)
	{
		printf("Model (%s) failed to load: %s", fileName, importer.GetErrorString());
		return;
	}

	aiMatrix4x4 mat;
	LoadNode(scene->mRootNode, scene, mat);
	LoadMaterials(scene);
	LoadShader();

	importer.FreeScene();
}

void StaticMesh::Update(float deltaTime)
{
	mAngle += deltaTime * 0.5f;
}

void StaticMesh::RenderModel(std::shared_ptr<Camera> camera)
{
	mMeshShader->SetActive();
	glm::mat4 mat = mPosMat * mRotMat * mScaMat;
	glm::mat4 mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * mat;
	
	SetUniform();
	mMeshShader->SetMatrixUniform("mvp_matrix", mvp);
	mMeshShader->SetVectorUniform("lightDir", glm::vec3(0.0f, 0.0f, 1.0f));

	for (size_t i = 0; i < mMeshList.size(); ++i)
	{
		unsigned int materialIndex = mMeshToTex[i];
		if (materialIndex < mTextureList.size() && mTextureList[materialIndex])
		{
			glActiveTexture(GL_TEXTURE0);
			mTextureList[materialIndex]->SetActive();
		}

		mMeshList[i]->SetActive();
		glDrawElements(GL_TRIANGLES, mMeshList[i]->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void StaticMesh::ClearModel()
{
}

void StaticMesh::LoadNode(aiNode* node, const aiScene* scene, const aiMatrix4x4& mat)
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

void StaticMesh::LoadMesh(aiMatrix4x4 mat, aiMesh* mesh, const aiScene * scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; ++i)
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

	for (size_t i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	
	std::shared_ptr<VertexArray> vertex 
		= std::make_shared<VertexArray>(&vertices[0], mesh->mNumVertices, &indices[0], indices.size());
	
	mMeshList.push_back(vertex);
	mMeshToTex.push_back(mesh->mMaterialIndex);
}

void StaticMesh::LoadMaterials(const aiScene * scene)
{
	mTextureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; ++i)
	{
		aiMaterial* material = scene->mMaterials[i];
		mTextureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);

				std::string texPath = "Assets/Model/"+ filename;

				mTextureList[i] = std::make_shared<Texture>();
				mTextureList[i]->Load(texPath.c_str());
			}
		}
	}
}

void StaticMesh::LoadShader()
{
	std::vector<std::pair<std::string, int> > shaderCodies;
	shaderCodies.push_back(make_pair(ReadShaderFile("mesh.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("mesh.frag"), GL_FRAGMENT_SHADER));
	mMeshShader = std::make_shared<Shader>();
	mMeshShader->BuildShader(shaderCodies);
}

void StaticMesh::SetUniform()
{
}
