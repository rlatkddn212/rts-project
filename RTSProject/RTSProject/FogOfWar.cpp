#include "Precompiled.h"
#include "FogOfWar.h"
#include "Shader.h"

FogOfWar::FogOfWar()
{
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
		0.0f,  1.0f,
		1.0f,  1.0f,
	};

	// 버텍스 버퍼에 핸들
	GLuint vertexArray2;
	glGenVertexArrays(1, &vertexArray2);
	glBindVertexArray(vertexArray2);
	// 버퍼를 생성
	GLuint mVertexbuffer;
	glGenBuffers(1, &mVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexbuffer);
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

	GLuint mUvBuffer;
	glGenBuffers(1, &mUvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mUvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,					// 1번째 속성(attribute).
		2,                  // 크기(size)
		GL_FLOAT,           // 타입(type)
		GL_FALSE,           // normalized
		0,                  // 다음 요소 까지 간격
		(void*)0            // 배열 버퍼의 오프셋
	);

	int mWidth;
	int mHeight;
	int channels;

	unsigned char* bytes = new unsigned char[64 * 64 * 3];

	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("no");
	}

	//랜더링
	std::vector<std::pair<std::string, int> > shaderCodies2;
	Shader shader2;

	shaderCodies2.clear();
	shaderCodies2.push_back(make_pair(ReadShaderFile("st.vert"), GL_VERTEX_SHADER));
	shaderCodies2.push_back(make_pair(ReadShaderFile("sight.frag"), GL_FRAGMENT_SHADER));
	shader2.BuildShader(shaderCodies2);

	static const GLfloat green[] = { 1.0f, 0.25f, 1.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, green);
	glBindVertexArray(vertexArray2);
	shader2.SetActive();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	unsigned int framebuffer2;
	glGenFramebuffers(1, &framebuffer2);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);

	// create a color attachment texture
	unsigned int textureColorbuffer2;
	glGenTextures(1, &textureColorbuffer2);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer2, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("no");
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer2);
	glBlitFramebuffer(0, 0, 1024, 768, 0, 0, 64, 64, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
	glReadPixels(0, 0, 64, 64, GL_RGB, GL_UNSIGNED_BYTE, bytes);

	SOIL_save_image("hello.bmp", SOIL_SAVE_TYPE_BMP, 64, 64, 3, bytes);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FogOfWar::Update(float deltaTime)
{
}

void FogOfWar::Render()
{

}
