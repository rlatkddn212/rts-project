#include "Precompiled.h"
#include "MiniMap.h"

MiniMap::MiniMap()
{
	static const GLfloat vertexBufferData[] =
	{
		0.5f, 0.5f, 0.0f,
		1.0f, 0.5f, 0.0f,
		0.5f,  1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	};

	static const GLfloat uvBufferData[] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f,  1.0f,
		1.0f,  1.0f,
	};

	// ���ؽ� ���ۿ� �ڵ�
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);
	// ���۸� ����
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// ���ؽ����� OpenGL��
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // 0��° �Ӽ�(attribute).
		3,                  // ũ��(size)
		GL_FLOAT,           // Ÿ��(type)
		GL_FALSE,           // ����ȭ(normalized)?
		0,                  // ���� ��� ���� ����(stride)
		(void*)0            // �迭 ������ ������(offset; �ű�� ��)
	);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvBufferData), uvBufferData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,					// 1��° �Ӽ�(attribute).
		2,                  // ũ��(size)
		GL_FLOAT,           // Ÿ��(type)
		GL_FALSE,           // normalized
		0,                  // ���� ��� ���� ����
		(void*)0            // �迭 ������ ������
	);

	std::vector<std::pair<std::string, int> > shaderCodies;
	shaderCodies.push_back(make_pair(ReadShaderFile("minimap.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("minimap.frag"), GL_FRAGMENT_SHADER));
	mShader = std::make_shared<Shader>();
	mShader->BuildShader(shaderCodies);
}

MiniMap::~MiniMap()
{
}

void MiniMap::Update(float deltaTime)
{

}

void MiniMap::Render(std::shared_ptr<Camera> camera)
{
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(mVao);
	glActiveTexture(GL_TEXTURE0);
	mMapTexture->SetActive();
	glActiveTexture(GL_TEXTURE1);
	mFogTexture->SetActive();

	mShader->SetActive();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_DEPTH_TEST);
}
