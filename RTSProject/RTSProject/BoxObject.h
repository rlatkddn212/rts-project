#pragma once
#include "Precompiled.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Camera.h"

class BoxObject
{
public:
	BoxObject(glm::vec3 minV, glm::vec3 maxV);
	~BoxObject();

	void MakeModel(glm::vec3 minV, glm::vec3 maxV);
	void Render(std::shared_ptr<Camera> camera);

	void SetPosition(glm::vec3 p) { mPos = glm::translate(glm::mat4(1.0f), p);}
	void SetRotation(glm::vec3 r) { mRot = glm::yawPitchRoll(r.x, r.y, r.z); }
	void SetScale(glm::vec3 s) { mSca = glm::scale(glm::mat4(1.0f), s); }

	bool RayBoxIntersect(const glm::vec3& orig, const glm::vec3& dir);
	bool RayTriangleIntersect(const glm::vec3& orig, const glm::vec3& dir,
		const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);

	glm::mat4 mPos, mRot, mSca;

	Shader* mMeshShader;
	VertexArray* mesh;
	glm::vec3 mMinV;
	glm::vec3 mMaxV;
};

