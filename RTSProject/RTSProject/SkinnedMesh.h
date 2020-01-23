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
	void									RenderModelShadow(std::shared_ptr<Camera> camera);
	
	void									ClearModel();

	// Load Object
	void									LoadModel(const std::string& fileName);
	void									LoadNode(aiNode *node, const aiScene *scene);
	void									LoadMesh(aiMatrix4x4 mat, aiMesh *mesh, const aiScene *scene);
	void									LoadMaterials(const aiScene *scene);

	// 변환 행렬
	void									SetPosition(glm::vec3 p) { 	mPos = glm::translate(glm::mat4(1.0f), p); }
	void									SetRotation(glm::vec3 r) { mRot = glm::yawPitchRoll(r.x, r.y, r.z); }
	void									SetScale(glm::vec3 s) { mSca = glm::scale(glm::mat4(1.0f), s); }

	glm::mat4								GetPositionMat() { return mPos; }
	glm::mat4								GetRotationMat() { return mRot; }
	glm::mat4								GetScaleMat() { return mSca; }

	// Box Object
	glm::vec3								GetMinPos() { return mMinPos; }
	glm::vec3								GetMaxPos() { return mMaxPos; }

	// 뷰 프로젝션
	void									SetPerspect(glm::mat4 p) { perspect = p; }
	void									SetView(glm::mat4 v) { view = v; }

	// read Bone
	void									ReadNodeHeirarchy(const aiAnimation* pAnimation, float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);
	void									BoneTransform();

	void									SetColor(glm::vec4 color) { mTeamColor = color; }
	
	double									GetAnimationTime(int idx) { return mScene->mAnimations[mAnimationIdx]->mDuration; }
	void									SetAnimationLoop(bool isLoop) { mIsAniLoop = isLoop; };
	bool									IsLoop() { return mIsAniLoop; }

	void									SetAnimation(int idx);
	void									SetAniSpeed(double speed) { mAniSpeed = speed; }
	void									SetAniTime(double time) { mAnimTime = time; }

	
	void									SetAnimationEnd(bool isEnd) { mIsAniEnd = isEnd; };
	bool									IsAnimationEnd() { return mIsAniEnd; }

protected:
	const static int						MAX_BONES = 100;

	glm::vec4								mTeamColor;
	// box Object
	glm::vec3								mMinPos;
	glm::vec3								mMaxPos;

	std::vector<glm::mat4>					mTransforms;
	glm::mat4								mPos;
	glm::mat4								mRot;
	glm::mat4								mSca;

	std::shared_ptr<Shader>					mShadowShader;
	std::shared_ptr<Shader>					mMeshShader;
	std::vector<std::shared_ptr<SkinnedVertexArray> > meshList;
	std::vector< std::shared_ptr<Texture> > textureList;
	std::vector<unsigned int>				meshToTex;

	std::vector<Bone>						mBoneInfo;
	glm::mat4								mGlobalInvBindPoses;
	
	GLuint									mBoneLocation[MAX_BONES];

	std::map<std::string, int>				mBoneMapping;
	int										mNumBones;

	// Assimp 파일 로더
	Assimp::Importer						importer;
	const aiScene*							mScene;
	
	float									mAngle;
	glm::mat4								perspect;
	glm::mat4								view;
	double									mAnimTime;
	double									mAniSpeed;

	int										mAnimationIdx;
	bool									mIsAniLoop;
	bool									mIsAniEnd;

	double									mX;
	double									mY;
	double									mZ;
	int										mSize;
};