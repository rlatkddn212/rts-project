#pragma once

#include <vector>
#include <string>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "Texture.h"
#include "SkinnedVertexArray.h"
#include "Shader.h"
#include <map>
#include "BoxObject.h"
#include "Camera.h"
#include "Ray.h"

struct Bone
{
	std::string mName;
	glm::mat4 mBindPose;
	int mParent;
};

struct VertexBoneData
{
	size_t IDs[4];
	float Weights[4];

	VertexBoneData()
	{
		Reset();
	};

	void Reset()
	{
		memset(IDs, 0, sizeof(IDs));
		memset(Weights, 0, sizeof(Weights));
	}

	void AddBoneData(size_t BoneID, float Weight);
};

class SkinnedMesh
{
public:
											SkinnedMesh();
										   ~SkinnedMesh();

	// Render Loop
	void									Update(float deltaTime);
	void									RenderModel(std::shared_ptr<Camera> camera);
	
	void									ClearModel();

	// Load Object
	void									LoadModel(const std::string& fileName);
	void									LoadNode(aiNode *node, const aiScene *scene);
	void									LoadMesh(aiMatrix4x4 mat, aiMesh *mesh, const aiScene *scene);
	void									LoadMaterials(const aiScene *scene);

	// 변환 행렬
	void									SetPosition(glm::vec3 p) { 
												mBoxObject->SetPosition(p);
												mPos = glm::translate(glm::mat4(1.0f), p);  
											}
	void									SetRotation(glm::vec3 r) { mRot = glm::yawPitchRoll(r.x, r.y, r.z); }
	void									SetScale(glm::vec3 s) { mSca = glm::scale(glm::mat4(1.0f), s); }

	// 뷰 프로젝션
	void									SetPerspect(glm::mat4 p) { perspect = p; }
	void									SetView(glm::mat4 v) { view = v; }

	// read Bone
	void									ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);
	void									BoneTransform();

	void									Select();
	void									UnSelect();

	bool									Intersect(Ray ray);
	glm::vec2								GetScreenPos(std::shared_ptr<Camera> camera);

	// box Object
	glm::vec3								mMinPos;
	glm::vec3								mMaxPos;
	std::shared_ptr<BoxObject>				mBoxObject;

	std::vector<glm::mat4>					Transforms;
	glm::mat4								mPos, mRot, mSca;

	std::shared_ptr<Shader>					mMeshShader;
	std::vector<std::shared_ptr<SkinnedVertexArray> > meshList;
	std::vector< std::shared_ptr<Texture> > textureList;
	std::vector<unsigned int>				meshToTex;

	std::vector<Bone>						mBoneInfo;
	glm::mat4								mGlobalInvBindPoses;
	
	const static int						MAX_BONES = 100;
	GLuint									mBoneLocation[MAX_BONES];

	std::map<std::string, int>				mBoneMapping;
	int										mNumBones;
	Assimp::Importer						importer;
	const aiScene*							scene;
	float									m_angle;
	glm::mat4								perspect;
	glm::mat4								view;
	float									mAnimTime;

	double									mX;
	double									mY;
	double									mZ;
	int										mSize;
};