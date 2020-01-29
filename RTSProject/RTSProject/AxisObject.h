#pragma once

#include "Precompiled.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Camera.h"
#include "RenderObject.h"
#include "RenderManager.h"

class AxisObject : public  RenderObject
{
public:
											AxisObject(int type) {}
											AxisObject();
									       ~AxisObject();

	void									MakeModel();
	void									Render(std::shared_ptr<Camera> camera);
	void									AddRender(std::shared_ptr<Camera> camera);

	void									SetPosition(glm::vec3 p) { mPosMat = glm::translate(glm::mat4(1.0f), p); }
	void									SetRotation(glm::vec3 r) { mRotMat = glm::yawPitchRoll(r.x, r.y, r.z); }
	void									SetScale(glm::vec3 s) { mScaMat = glm::scale(glm::mat4(1.0f), s); }

	glm::mat4								mPosMat, mRotMat, mScaMat;
	std::shared_ptr<Shader>					mMeshShader;

	unsigned int							mVertexArray;
	unsigned int							mVertexBuffer;
};