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
	mFont = std::make_shared<Font>();
	mFont->Initialize();
	t = mFont->RenderText("hello");

	float vertices[] = {
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
		0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = std::make_shared<VertexArray>(vertices, 4, indices, 6);
	
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

	// 버텍스 버퍼에 핸들

	// 버퍼를 생성
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// 버텍스들을 OpenGL로
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // 0번째 속성(attribute).
		3,                  // 크기(size)
		GL_FLOAT,           // 타입(type)
		GL_FALSE,           // 정규화(normalized)?
		0,                  // 다음 요소 까지 간격(stride)
		(void*)0            // 배열 버퍼의 오프셋(offset; 옮기는 값)
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,					// 1번째 속성(attribute).
		2,                  // 크기(size)
		GL_FLOAT,           // 타입(type)
		GL_FALSE,           // normalized
		0,                  // 다음 요소 까지 간격
		(void*)0            // 배열 버퍼의 오프셋
	);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data, GL_STATIC_DRAW);
	
	std::vector<std::pair<std::string, int> > shaderCodies;
	shaderCodies.push_back(make_pair(ReadShaderFile("vertex.glsl"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("Image.glsl"), GL_FRAGMENT_SHADER));
	
	mBackgroundShader = std::make_shared<Shader>();
	mSpriteShader = std::make_shared<Shader>();

	mBackgroundShader->BuildShader(shaderCodies);
	shaderCodies.clear();
	shaderCodies.push_back(make_pair(ReadShaderFile("Sprite.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("Sprite.frag"), GL_FRAGMENT_SHADER));
	mSpriteShader->BuildShader(shaderCodies);
}

void WindowGroup::Terminate()
{
}

void WindowGroup::Update()
{

}

void WindowGroup::Draw()
{
	// drawing
	mBackgroundShader->SetActive();
	DrawBackGround();
	mSpriteVerts->SetActive();
	mSpriteShader->SetActive();
	
	const glm::vec2 cRadarPos(100.0f, 0.0f);
	DrawTexture(t, cRadarPos, 1.0f);
}

void WindowGroup::DrawTexture(std::shared_ptr<Texture> texture, const glm::vec2& offset, float scale)
{
	// Scale the quad by the width/height of texture
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.f),
		glm::vec3( static_cast<float>(texture->GetWidth()) * scale,
		static_cast<float>(texture->GetHeight()) * scale,
		1.0f));
	// Translate to position on screen
	glm::mat4 transMat = glm::translate(glm::mat4(1.f), glm::vec3(offset.x, offset.y, 0.0f));
	// Set world transform
		glm::mat4 m = {
			{ 2.0f / 1024.f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2.0f / 768.f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 1.0f }
	};
	//glm::mat4();
	mSpriteShader->SetMatrixUniform("uViewProj", m);

	glm::mat4 world = transMat * scaleMat;
	mSpriteShader->SetMatrixUniform("uWorldTransform", world);
	// Set current texture
	texture->SetActive();
	// Draw quad
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void WindowGroup::DrawBackGround()
{
	glBindVertexArray(VertexArrayID);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
