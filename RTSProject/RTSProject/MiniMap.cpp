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

	// 버텍스 버퍼에 핸들
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);
	// 버퍼를 생성
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// 버텍스들을 OpenGL로
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // 0번째 속성(attribute).
		3,                  // 크기(size)
		GL_FLOAT,           // 타입(type)
		GL_FALSE,           // 정규화(normalized)?
		0,                  // 다음 요소 까지 간격(stride)
		(void*)0            // 배열 버퍼의 오프셋(offset; 옮기는 값)
	);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvBufferData), uvBufferData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,					// 1번째 속성(attribute).
		2,                  // 크기(size)
		GL_FLOAT,           // 타입(type)
		GL_FALSE,           // normalized
		0,                  // 다음 요소 까지 간격
		(void*)0            // 배열 버퍼의 오프셋
	);

	std::vector<std::pair<std::string, int> > shaderCodies;
	shaderCodies.push_back(make_pair(ReadShaderFile("minimap.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("minimap.frag"), GL_FRAGMENT_SHADER));
	mShader = std::make_shared<Shader>();
	mShader->BuildShader(shaderCodies);
	
	mPositionTexture = std::make_shared<Texture>();
	mPositionTexture->CreateTexture(100, 100, 0);
	
	glGenFramebuffers(1, &mFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPositionTexture->GetTextureID(), 0);

	// Frame buffer 완전성 위배
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return;
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

MiniMap::~MiniMap()
{
	glDeleteFramebuffers(1, &mFrameBuffer);
}

void MiniMap::Update(float deltaTime, std::vector<std::shared_ptr<Unit>> unit)
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
	// visible 텍스쳐 생성
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat green[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, black);
	glEnable(GL_SCISSOR_TEST);
	for (size_t unitIdx = 0; unitIdx < unit.size(); ++unitIdx)
	{
		glm::vec3 pos = unit[unitIdx]->GetPosition();

		glm::vec2 screen = glm::vec2(pos.x, -pos.z);

		glScissor(screen.x, screen.y, 1, 1);
		glClearBufferfv(GL_COLOR, 0, green);
	}

	glDisable(GL_SCISSOR_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MiniMap::Render(std::shared_ptr<Camera> camera)
{
	mShader->SetActive();
	
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(mVao);
	glActiveTexture(GL_TEXTURE0);
	mMapTexture->SetActive();
	glActiveTexture(GL_TEXTURE1);
	mFogTexture->SetActive();
	glActiveTexture(GL_TEXTURE2);
	mPositionTexture->SetActive();


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_DEPTH_TEST);
}
