#pragma once

#include <vector>
#include <string>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "Texture.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"

class StaticMesh
{
public:
											StaticMesh();
										   ~StaticMesh();

	void									LoadModel(const std::string& fileName);
	void									Update(float deltaTime);
	void									RenderModel(std::shared_ptr<Camera> camera);
	void									ClearModel();

	void									LoadNode(aiNode *node, const aiScene *scene, const aiMatrix4x4& mat);
	void									LoadMesh(aiMatrix4x4 mat, aiMesh *mesh, const aiScene *scene);
	void									LoadMaterials(const aiScene *scene);

	void									SetPosition(glm::vec3 p) { mPos = glm::translate(glm::mat4(1.0f), p); }
	void									SetRotation(glm::vec3 r) { mRot = glm::yawPitchRoll(r.x, r.y, r.z); }
	void									SetScale(glm::vec3 s) { mSca = glm::scale(glm::mat4(1.0f), s); }

	void									SetPerspect(glm::mat4 p) { mPerspect = p; }
	void									SetView(glm::mat4 v) { mView = v; }

	glm::mat4								mPos, mRot, mSca;
	std::shared_ptr<Shader>					mMeshShader;
	std::vector<std::shared_ptr<VertexArray> > mMeshList;
	std::vector< std::shared_ptr<Texture> > mTextureList;
	std::vector<unsigned int>				mMeshToTex;

	float									mAngle;
	glm::mat4								mPerspect;
	glm::mat4								mView;
};
