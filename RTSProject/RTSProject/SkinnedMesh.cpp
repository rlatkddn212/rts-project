#include "Precompiled.h"
#include "SkinnedMesh.h"
#include "SkinnedVertexArray.h"

SkinnedMesh::SkinnedMesh()
{
	mAnimTime = 0.0f;
	m_angle = 0.0f;
	mNumBones = 0;
	SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
	SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));

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

	// 오브젝트 중심
	printf("%f %f %f", mX / mSize, mY / mSize, mZ / mSize);

	mBoxObject = new BoxObject(mMinPos, mMaxPos);


	std::vector<std::pair<std::string, int> > shaderCodies;
	shaderCodies.push_back(make_pair(ReadShaderFile("skinned.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("skinned.frag"), GL_FRAGMENT_SHADER));
	mMeshShader = new Shader();
	mMeshShader->BuildShader(shaderCodies);
}

void SkinnedMesh::Update(float deltaTime)
{
	mAnimTime += deltaTime;
	m_angle += deltaTime * 0.5f;

	float aspect = (float)1024 / (float)768;
	perspect = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 1000.0f);
	view = glm::lookAt(glm::vec3(cos(m_angle) * 2.0f, 1.5f, sin(m_angle) * 2.0f),
		glm::vec3(0.0f, 0.5f, 0.0f),
		glm::vec3(0.0, 1.0, 0.0));
}

void SkinnedMesh::RenderModel(Camera* camera)
{
	mBoxObject->Render(camera);

	float aspect = (float)1024 / (float)768;

	glm::mat4 model = mPos * mRot * mSca;
	glm::mat4 mat = camera->GetProjectionMatrix() * camera->GetViewMatrix() * model;

	mMeshShader->SetActive();

	mMeshShader->SetVectorUniform("lightDir", glm::vec3(1.0f, 0.0f, 0.0f));
	mMeshShader->SetVector4Uniform("teamColor", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	mMeshShader->SetMatrixUniform("mvp_matrix", mat);
	mMeshShader->SetMatrixUniform("model", model);
	mMeshShader->SetMatrixUniform("view", camera->GetViewMatrix());
	mMeshShader->SetMatrixUniform("perspect", camera->GetProjectionMatrix());

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

		printf("%f %f %f\n", mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		// 정점
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y , mesh->mVertices[i].z});
		// 노멀
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
		
		// 텍스쳐 좌표
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

	SkinnedVertexArray* vertex = new SkinnedVertexArray(&vertices[0], mesh->mNumVertices, &indices[0], indices.size());
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

				std::string texPath = filename;

				textureList[i] = new Texture();
				textureList[i]->Load(texPath.c_str());
			}
		}
	}
}

void SkinnedMesh::ReadNodeHeirarchy(float AnimationTime, const aiNode * pNode, const glm::mat4 & ParentTransform)
{
	std::string NodeName(pNode->mName.data);

	const aiAnimation* pAnimation = scene->mAnimations[0];
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
		
		// 스케일
		for (size_t i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) 
		{
			if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
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
			if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
				frame = i;
				break;
			}
		}

		DeltaTime = pNodeAnim->mPositionKeys[frame + 1].mTime - pNodeAnim->mPositionKeys[frame].mTime;
		Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[frame].mTime) / DeltaTime;
		p1 = pNodeAnim->mPositionKeys[frame].mValue;
		p2 = pNodeAnim->mPositionKeys[frame + 1].mValue;

		glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), glm::lerp(glm::vec3(p1.x, p1.y, p1.z), glm::vec3(p2.x, p2.y, p2.z), Factor));

		// 회전
		for (size_t i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
		{
			if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
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
	else
	{
		int a = 0;
	}

	glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

	if (mBoneMapping.find(NodeName) != mBoneMapping.end())
	{
		size_t BoneIndex = mBoneMapping[NodeName];
		Transforms[BoneIndex] = mGlobalInvBindPoses * GlobalTransformation * mBoneInfo[BoneIndex].mBindPose;
	}

	for (size_t i = 0; i < pNode->mNumChildren; i++) 
	{
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}
}

void SkinnedMesh::BoneTransform()
{
	glm::mat4 Identity = glm::identity<glm::mat4>();

	float TicksPerSecond = (float)(scene->mAnimations[0]->mTicksPerSecond != 0 ? scene->mAnimations[0]->mTicksPerSecond : 25.0f);
	float TimeInTicks = mAnimTime * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, (float)scene->mAnimations[0]->mDuration);

	Transforms.resize(100);	
	ReadNodeHeirarchy(AnimationTime, scene->mRootNode, Identity);
}

bool SkinnedMesh::Intersect(Ray ray)
{
	glm::mat4 world = mPos * mRot * mSca;
	glm::mat4 worldInv = glm::inverse(world);
	glm::vec3 pos = worldInv * glm::vec4(ray.org, 1.0f);
	glm::vec3 dir = worldInv * glm::vec4(ray.dir, 0.0f);
	dir = glm::normalize(dir);

	if (mBoxObject->RayBoxIntersect(pos, dir))
	{
		std::cout << "ray" << std::endl;
	}

	return false;
}



void VertexBoneData::AddBoneData(size_t BoneID, float Weight)
{
	for (size_t i = 0; i < 4; i++) {
		if (Weights[i] == 0.0) {
			IDs[i] = BoneID;
			Weights[i] = Weight;
			return;
		}
	}

	assert(0);
}
