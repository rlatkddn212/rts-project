#include "Precompiled.h"
#include "SkinnedMesh.h"
#include "SkinnedVertexArray.h"

SkinnedMesh::SkinnedMesh()
{
	mAnimTime = 0.0f;
	m_angle = 0.0f;
	mNumBones = 0;
	mAnimationIdx = 0;
	mMinPos = glm::vec3(2000.0f, 2000.0f, 2000.0f);
	mMaxPos = glm::vec3(-2000.0f, -2000.0f, -2000.0f);
}

SkinnedMesh::~SkinnedMesh()
{
}

void SkinnedMesh::LoadModel(const std::string & fileName)
{	
	scene = importer.ReadFile(fileName, 
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

	LoadNode(scene->mRootNode, scene);
	LoadMaterials(scene);

	SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
	SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));

	std::vector<std::pair<std::string, int> > shaderCodies;
	shaderCodies.push_back(make_pair(ReadShaderFile("skinned.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("skinned.frag"), GL_FRAGMENT_SHADER));
	mMeshShader = std::make_shared<Shader>();
	mMeshShader->BuildShader(shaderCodies);

	//importer.FreeScene();
}

void SkinnedMesh::Update(float deltaTime)
{
	mAnimTime += deltaTime;
	m_angle += deltaTime * 0.5f;
}

void SkinnedMesh::RenderModel(std::shared_ptr<Camera> camera)
{
	glm::mat4 model = mPos * mRot * mSca;
	glm::mat4 mat = camera->GetProjectionMatrix() * camera->GetViewMatrix();

	mMeshShader->SetActive();

	mMeshShader->SetVectorUniform("lightDir", glm::vec3(0.0f, 1.0f, 0.0f));
	mMeshShader->SetVector4Uniform("teamColor", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	mMeshShader->SetMatrixUniform("vpMatrix", mat);
	mMeshShader->SetMatrixUniform("worldMatrix", model);

	for (int i = 0; i < 100; ++i)
	{
		char Name[128];
		memset(Name, 0, sizeof(Name));
		snprintf(Name, sizeof(Name), "gBones[%d]", i);
		mMeshShader->SetMatrixUniform(Name, Transforms[i]);
	}

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

void SkinnedMesh::ClearModel()
{
}

void SkinnedMesh::LoadNode(aiNode * node, const aiScene * scene)
{
	aiMatrix4x4 m_GlobalInverseTransform = scene->mRootNode->mTransformation;

	memcpy(&mGlobalInvBindPoses, &m_GlobalInverseTransform, sizeof(m_GlobalInverseTransform));
	mGlobalInvBindPoses = glm::transpose(mGlobalInvBindPoses);
	mGlobalInvBindPoses = glm::inverse(mGlobalInvBindPoses);

	for (size_t i = 0; i < node->mNumMeshes; ++i)
	{
		LoadMesh(node->mTransformation, scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; ++i)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

void SkinnedMesh::LoadMesh(aiMatrix4x4 mat, aiMesh * mesh, const aiScene * scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	std::vector<VertexBoneData> mBones(mesh->mNumVertices);
	for (size_t i = 0; i < mesh->mNumBones; i++)
	{
		size_t BoneIndex = 0;
		
		std::string BoneName(mesh->mBones[i]->mName.data);

		if (mBoneMapping.find(BoneName) == mBoneMapping.end())
		{
			// Allocate an index for a new bone
			BoneIndex = mNumBones;
			mNumBones++;

			Bone tmp;
			aiMatrix4x4& aiMat = mesh->mBones[i]->mOffsetMatrix;
			memcpy(&tmp.mBindPose, &aiMat[0][0], sizeof(tmp.mBindPose));
			tmp.mBindPose = glm::transpose(tmp.mBindPose);
			Bone bi;
			mBoneInfo.push_back(bi);
			mBoneInfo[BoneIndex].mBindPose = tmp.mBindPose;
			mBoneMapping[BoneName] = BoneIndex;
		}
		else 
		{
			BoneIndex = mBoneMapping[BoneName];
		}

		for (size_t j = 0; j < mesh->mBones[i]->mNumWeights; ++j) {
			size_t VertexID = mesh->mBones[i]->mWeights[j].mVertexId;
			float Weight = mesh->mBones[i]->mWeights[j].mWeight;
			mBones[VertexID].AddBoneData(BoneIndex, Weight);
		}
	}

	mSize += mesh->mNumVertices;
	for (size_t i = 0; i < mesh->mNumVertices; ++i)
	{
		mX += mesh->mVertices[i].x;
		mY += mesh->mVertices[i].y;
		mZ += mesh->mVertices[i].z;
		
		aiVector3D minPos = mat * mesh->mVertices[i];
		//aiVector3D minPos = mesh->mNormals[i] = mat * mesh->mNormals[i];

		mMinPos.x = glm::min(mMinPos.x, minPos.x);
		mMinPos.y = glm::min(mMinPos.y, minPos.y);
		mMinPos.z = glm::min(mMinPos.z, minPos.z);
		
		mMaxPos.x = glm::max(mMaxPos.x, minPos.x);
		mMaxPos.y = glm::max(mMaxPos.y, minPos.y);
		mMaxPos.z = glm::max(mMaxPos.z, minPos.z);

		// ����
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y , mesh->mVertices[i].z});
		// ���
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
		
		for (size_t j = 0; j < 4; ++j)
		{
			GLfloat t;
			memcpy(&t, &(mBones[i].IDs[j]), sizeof(t));
			vertices.push_back(t);
		}

		for (size_t j = 0; j < 4; ++j)
		{
			vertices.push_back(mBones[i].Weights[j]);
		}
		
		// �ؽ��� ��ǥ
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else
		{
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

	std::shared_ptr<SkinnedVertexArray> vertex = 
		std::make_shared<SkinnedVertexArray>(&vertices[0], mesh->mNumVertices, &indices[0], indices.size());
	meshList.push_back(vertex);
	meshToTex.push_back(mesh->mMaterialIndex);
}

void SkinnedMesh::LoadMaterials(const aiScene * scene)
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

				std::string texPath = "Assets/Model/" + filename;

				textureList[i] = std::make_shared<Texture>();
				textureList[i]->Load(texPath.c_str());
			}
		}
	}
}

void SkinnedMesh::ReadNodeHeirarchy(const aiAnimation* pAnimation, float AnimationTime, const aiNode * pNode, const glm::mat4 & ParentTransform)
{
	std::string NodeName(pNode->mName.data);
	const aiNodeAnim* pNodeAnim = nullptr;

	bool isFind = false;
	for (size_t i = 0; i < pAnimation->mNumChannels; ++i)
	{
		pNodeAnim = pAnimation->mChannels[i];
		if (std::string(pNodeAnim->mNodeName.data) == NodeName)
		{
			isFind = true;
			break;
		}
	}
	if (!isFind)pNodeAnim = nullptr;
	
	glm::mat4 NodeTransformation = glm::identity<glm::mat4>();
	memcpy(&NodeTransformation, &pNode->mTransformation, sizeof(NodeTransformation));
	NodeTransformation = glm::transpose(NodeTransformation);

	if (pNodeAnim) 
	{
		aiVector3D Scaling;
		int frame = 0;
		
		// ������
		for (size_t i = 0; i < pNodeAnim->mNumScalingKeys - 1; ++i) 
		{
			if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) 
			{
				frame = i;
				break;
			}
		}

		float DeltaTime = pNodeAnim->mPositionKeys[frame + 1].mTime - pNodeAnim->mPositionKeys[frame].mTime;
		float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[frame].mTime) / DeltaTime;
		aiVector3D p1 = pNodeAnim->mScalingKeys[frame].mValue;
		aiVector3D p2 = pNodeAnim->mScalingKeys[frame + 1].mValue;
		
		glm::mat4 ScalingM = glm::scale(glm::mat4(1.0f), glm::lerp(glm::vec3(p1.x, p1.y, p1.z), glm::vec3(p2.x, p2.y, p2.z), Factor));
	
		for (size_t i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
		{
			if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
			{
				frame = i;
				break;
			}
		}

		DeltaTime = pNodeAnim->mPositionKeys[frame + 1].mTime - pNodeAnim->mPositionKeys[frame].mTime;
		Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[frame].mTime) / DeltaTime;
		p1 = pNodeAnim->mPositionKeys[frame].mValue;
		p2 = pNodeAnim->mPositionKeys[frame + 1].mValue;

		glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), glm::lerp(glm::vec3(p1.x, p1.y, p1.z), glm::vec3(p2.x, p2.y, p2.z), Factor));

		// ȸ��
		for (size_t i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
		{
			if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
			{
				frame = i;
				break;
			}
		}

		DeltaTime = pNodeAnim->mPositionKeys[frame + 1].mTime - pNodeAnim->mPositionKeys[frame].mTime;
		Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[frame].mTime) / DeltaTime;
		aiQuaternion q1 = pNodeAnim->mRotationKeys[frame].mValue;
		aiQuaternion q2 = pNodeAnim->mRotationKeys[frame + 1].mValue;
		glm::quat q = glm::slerp(glm::quat(q1.w, q1.x, q1.y, q1.z), glm::quat(q2.w, q2.x, q2.y, q2.z), Factor);
		glm::mat4 RotationM = glm::toMat4(q);

		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

	if (mBoneMapping.find(NodeName) != mBoneMapping.end())
	{
		size_t BoneIndex = mBoneMapping[NodeName];
		Transforms[BoneIndex] = mGlobalInvBindPoses * GlobalTransformation * mBoneInfo[BoneIndex].mBindPose;
	}

	for (size_t i = 0; i < pNode->mNumChildren; i++) 
	{
		ReadNodeHeirarchy(pAnimation, AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}
}

void SkinnedMesh::BoneTransform()
{
	glm::mat4 Identity = glm::identity<glm::mat4>();
	const aiAnimation* pAnimation = scene->mAnimations[mAnimationIdx];
	float TicksPerSecond = (float)(pAnimation->mTicksPerSecond != 0 ? pAnimation->mTicksPerSecond : 25.0f);
	float TimeInTicks = mAnimTime * 100 * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, (float)pAnimation->mDuration);

	Transforms.resize(100);	
	ReadNodeHeirarchy(pAnimation, AnimationTime, scene->mRootNode, Identity);
}

void VertexBoneData::AddBoneData(size_t BoneID, float Weight)
{
	for (size_t i = 0; i < 4; i++) 
	{
		if (Weights[i] == 0.0) 
		{
			IDs[i] = BoneID;
			Weights[i] = Weight;
			return;
		}
	}

	assert(0);
}
