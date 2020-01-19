#include "Precompiled.h"
#include "AxisObject.h"
#include "RenderManager.h"
#include "RenderObject.h"
#include "ObjectResourcePool.h"

using namespace std;

AxisObject::AxisObject()
{
	mRenderState = ForwardRendering;
	SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	std::shared_ptr<AxisObject> mini = ObjectResourcePool::GetInstance()->GetAxisObject();

	mMeshShader = mini->mMeshShader;
	mVertexArray = mini->mVertexArray;
	mVertexBuffer = mini->mVertexBuffer;
}

AxisObject::~AxisObject()
{
}

void AxisObject::MakeModel()
{
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	unsigned vertexSize = 7 * sizeof(float);

	float verts[] =
	{
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		10.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 10.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 10.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
	};

	// Create vertex buffer
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * vertexSize, verts, GL_STATIC_DRAW);

	// Position is 3 floats
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

	// Color is 4 floats
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize,
		reinterpret_cast<void*>(sizeof(float) * 3));

	std::vector<std::pair<std::string, int> > shaderCodies;
	shaderCodies.push_back(make_pair(ReadShaderFile("color.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("color.frag"), GL_FRAGMENT_SHADER));
	mMeshShader = std::make_shared<Shader>();
	mMeshShader->BuildShader(shaderCodies);
}

void AxisObject::Render(std::shared_ptr<Camera> camera)
{
	glBindVertexArray(mVertexArray);
	mMeshShader->SetActive();
	glm::mat4 mat = camera->GetProjectionMatrix() * camera->GetViewMatrix();
	mMeshShader->SetMatrixUniform("vpMatrix", mat);
	mMeshShader->SetMatrixUniform("worldMatrix", glm::mat4(1.0f));
	glLineWidth(2.f);
	glDrawArrays(GL_LINES, 0, 6);
}

void AxisObject::AddRender(std::shared_ptr<Camera> camera)
{
	shared_ptr<AxisObject> ro = make_shared<AxisObject>(*this);
	ro->mCamera = camera;
	RenderManager::GetInstance()->AddQueue(ro);
}
