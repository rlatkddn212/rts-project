#include "Precompiled.h"
#include "BoxObject.h"

BoxObject::BoxObject(glm::vec3 minV, glm::vec3 maxV)
{
	mMaxV = maxV;
	mMinV = minV;
	MakeModel(minV, maxV);
	SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
}

BoxObject::~BoxObject()
{
}

void BoxObject::MakeModel(glm::vec3 minV, glm::vec3 maxV)
{
	float vert[64];
	unsigned int indices[36] =
	{
		0, 1, 2,
		1, 2, 3,
		1, 5 ,3,
		5, 3, 7,
		5, 4, 7,
		4, 7, 6,
		4, 0, 6,
		0, 6, 2,
		2, 3, 6,
		3, 6, 7,
		0, 1, 4,
		1, 4, 5
	};

	int numVerts = 8;
	int numIndices = 36;

	//glm::vec3 minV = glm::vec3(0,0,0);
	//glm::vec3 maxV = glm ::vec3(1.0f, 1.0f, 1.0f);

	std::vector<glm::vec3> v;
	v.push_back(minV);
	v.push_back(glm::vec3(maxV.x, minV.y, minV.z));
	v.push_back(glm::vec3(minV.x, maxV.y, minV.z));
	v.push_back(glm::vec3(maxV.x, maxV.y, minV.z));
	v.push_back(glm::vec3(minV.x, minV.y, maxV.z));
	v.push_back(glm::vec3(maxV.x, minV.y, maxV.z));
	v.push_back(glm::vec3(minV.x, maxV.y, maxV.z));
	v.push_back(maxV);

	for (int i = 0; i < v.size(); ++i)
	{
		vert[i * 8] = v[i].x;
		vert[i * 8 + 1] = v[i].y;
		vert[i * 8 + 2] = v[i].z;
	}

	mMesh = std::make_shared<VertexArray>(vert, numVerts, indices, numIndices);

	std::vector<std::pair<std::string, int> > shaderCodies;
	shaderCodies.push_back(make_pair(ReadShaderFile("mesh.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("line.frag"), GL_FRAGMENT_SHADER));
	mMeshShader = std::make_shared<Shader>();
	mMeshShader->BuildShader(shaderCodies);
}

// 랜더링
void BoxObject::Render(std::shared_ptr<Camera> camera)
{
	if (isVisiable)
	{
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glm::mat4 world = mPosMat * mRotMat * mScaMat;
		glm::mat4 mat = camera->GetProjectionMatrix() * camera->GetViewMatrix();
		mMeshShader->SetActive();
		mMeshShader->SetMatrixUniform("worldMatrix", world);
		mMeshShader->SetMatrixUniform("vpMatrix", mat);

		mMesh->SetActive();
		glDrawElements(GL_TRIANGLES, mMesh->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);
	}
}

void BoxObject::SetVisiable(bool t)
{
	isVisiable = t;
}

// Ray 체크
bool BoxObject::RayBoxIntersect(const glm::vec3 & orig, const glm::vec3 & dir)
{
	unsigned int indices[36] =
	{
		0, 1, 2,
		1, 3, 2,
		1, 5 ,3,
		5, 7, 3,
		5, 4, 7,
		4, 6, 7,
		4, 0, 6,
		0, 2, 6,
		2, 3, 6,
		3, 7, 6,
		0, 1, 4,
		1, 5, 4
	};

	int numVerts = 8;
	int numIndices = 36;

	std::vector<glm::vec3> v;
	v.push_back(mMinV);
	v.push_back(glm::vec3(mMaxV.x, mMinV.y, mMinV.z));
	v.push_back(glm::vec3(mMinV.x, mMaxV.y, mMinV.z));
	v.push_back(glm::vec3(mMaxV.x, mMaxV.y, mMinV.z));
	v.push_back(glm::vec3(mMinV.x, mMinV.y, mMaxV.z));
	v.push_back(glm::vec3(mMaxV.x, mMinV.y, mMaxV.z));
	v.push_back(glm::vec3(mMinV.x, mMaxV.y, mMaxV.z));
	v.push_back(mMaxV);

	for (int i = 0; i < 12; ++i)
	{
		if (RayTriangleIntersect(orig, dir, v[indices[i * 3]], v[indices[i * 3 + 1]], v[indices[i * 3 + 2]]))
		{
			return true;
		}
	}

	return false;
}

bool BoxObject::RayTriangleIntersect(
	const glm::vec3& orig, const glm::vec3& dir,
	const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
{
	glm::vec3 v0v1 = v1 - v0;
	glm::vec3 v0v2 = v2 - v0;
	glm::vec3 N = glm::cross(v0v1, v0v2);
	float area2 = N.length();

	float NdotRayDirection = glm::dot(N, dir);
	if (fabs(NdotRayDirection) < 0.00001)
	{
		return false; 
	}

	float t = glm::dot((v0 - orig), N) / NdotRayDirection;
	if (t < 0) return false;

	glm::vec3 P = orig + t * dir;

	glm::vec3 C;

	glm::vec3 edge0 = v1 - v0;
	glm::vec3 vp0 = P - v0;

	C = glm::cross(edge0, vp0);
	if (glm::dot(N, C) < 0) return false;

	glm::vec3 edge1 = v2 - v1;
	glm::vec3 vp1 = P - v1;
	C = glm::cross(edge1, vp1);
	if (glm::dot(N, C) < 0)  return false;

	glm::vec3 edge2 = v0 - v2;
	glm::vec3 vp2 = P - v2;
	C = glm::cross(edge2, vp2);
	if (glm::dot(N, C) < 0) return false;

	return true;
}