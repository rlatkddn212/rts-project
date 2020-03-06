#pragma once

#include "Shader.h"
#include "VertexArray.h"
#include "Camera.h"
#include "RenderObject.h"
#include "RenderManager.h"

class BoxObject : public RenderObject
{
public:
											BoxObject(glm::vec3 minV, glm::vec3 maxV);
										   ~BoxObject();

	void									MakeModel(glm::vec3 minV, glm::vec3 maxV);
	void									Render(std::shared_ptr<Camera> camera);
	void									AddRender(std::shared_ptr<Camera> camera);

	void									SetVisiable(bool t);
	
	void									SetPosition(glm::vec3 p) { mPosMat = glm::translate(glm::mat4(1.0f), p);}
	void									SetRotation(glm::vec3 r) { mRotMat = glm::yawPitchRoll(r.x, r.y, r.z); }
	void									SetScale(glm::vec3 s) { mScaMat = glm::scale(glm::mat4(1.0f), s); }

	bool									RayBoxIntersect(const glm::vec3& orig, const glm::vec3& dir);
	bool									RayTriangleIntersect(const glm::vec3& orig, const glm::vec3& dir,
												const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);

	bool									isVisiable;
	glm::mat4								mPosMat, mRotMat, mScaMat;

	std::shared_ptr<Shader>					mMeshShader;
	std::shared_ptr<VertexArray>			mMesh;
	glm::vec3								mMinV;
	glm::vec3								mMaxV;
};

