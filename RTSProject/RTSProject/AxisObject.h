#pragma once

#include "Precompiled.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Camera.h"

class AxisObject
{
public:
										AxisObject();
									   ~AxisObject();

	void								MakeModel();
	void								Render(std::shared_ptr<Camera> camera);

	void								SetPosition(glm::vec3 p) { mPos = glm::translate(glm::mat4(1.0f), p); }
	void								SetRotation(glm::vec3 r) { mRot = glm::yawPitchRoll(r.x, r.y, r.z); }
	void								SetScale(glm::vec3 s) { mSca = glm::scale(glm::mat4(1.0f), s); }

	glm::mat4							mPos, mRot, mSca;
	std::shared_ptr<Shader>				mMeshShader;

	unsigned int						mVertexArray;
	unsigned int						mVertexBuffer;
};

