#include "Precompiled.h"
#include "WindowGroup.h"
#include "Font.h"
#include "Shader.h"

WindowGroup::WindowGroup()
{
}

WindowGroup::~WindowGroup()
{
}

void WindowGroup::Initialize()
{
	mText = std::make_shared<TextUI>();
	setlocale(LC_ALL, "");
	const char* str = "�̰��� Font";
	mText->SetText(str);
	
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	static const GLfloat g_vertex_buffer_data[] =
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	};

	static const GLfloat uv_buffer_data[] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
	};

	// ���ؽ� ���ۿ� �ڵ�

	// ���۸� ����
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// ���ؽ����� OpenGL��
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // 0��° �Ӽ�(attribute).
		3,                  // ũ��(size)
		GL_FLOAT,           // Ÿ��(type)
		GL_FALSE,           // ����ȭ(normalized)?
		0,                  // ���� ��� ���� ����(stride)
		(void*)0            // �迭 ������ ������(offset; �ű�� ��)
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,					// 1��° �Ӽ�(attribute).
		2,                  // ũ��(size)
		GL_FLOAT,           // Ÿ��(type)
		GL_FALSE,           // normalized
		0,                  // ���� ��� ���� ����
		(void*)0            // �迭 ������ ������
	);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data, GL_STATIC_DRAW);
	
	std::vector<std::pair<std::string, int> > shaderCodies;
	shaderCodies.push_back(make_pair(ReadShaderFile("vertex.glsl"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("Image.glsl"), GL_FRAGMENT_SHADER));
	
	mBackgroundShader = std::make_shared<Shader>();
}

void WindowGroup::Terminate()
{
}

void WindowGroup::Update()
{

}

void WindowGroup::Render(std::shared_ptr<Camera> camera)
{
	mText->Render(camera);
	// drawing
	//mBackgroundShader->SetActive();
	//DrawBackGround();
}

void WindowGroup::DrawBackGround()
{
	glBindVertexArray(VertexArrayID);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
