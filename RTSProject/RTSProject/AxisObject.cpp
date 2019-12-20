#include "Precompiled.h"
#include "AxisObject.h"

AxisObject::AxisObject()
{
	SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
	
	MakeModel();
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
	mMeshShader->SetActive();
	glm::mat4 mat = camera->GetProjectionMatrix() * camera->GetViewMatrix();
	mMeshShader->SetMatrixUniform("mvp_matrix", mat);
	glBindVertexArray(mVertexArray);
	glLineWidth(2.f);
	glDrawArrays(GL_LINES, 0, 6);
}
