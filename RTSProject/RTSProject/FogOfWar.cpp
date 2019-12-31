#include "Precompiled.h"
#include "FogOfWar.h"
#include "Shader.h"
#include <vector>

FogOfWar::FogOfWar()
{
	static const GLfloat vertexBufferData[] =
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
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

	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	mSightTexture = std::make_shared<Texture>();
	mSightTexture->CreateTexture(1024, 768);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mSightTexture->GetTextureID(), 0);

	// Frame buffer ������ ����
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return;
	}

	//������
	std::vector<std::pair<std::string, int> > shaderCodies2;
	Shader sightShader;

	shaderCodies2.clear();
	shaderCodies2.push_back(make_pair(ReadShaderFile("st.vert"), GL_VERTEX_SHADER));
	shaderCodies2.push_back(make_pair(ReadShaderFile("sight.frag"), GL_FRAGMENT_SHADER));
	sightShader.BuildShader(shaderCodies2);

	static const GLfloat green[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glClearBufferfv(GL_COLOR, 0, green);
	sightShader.SetActive();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	unsigned char* bytes = new unsigned char[10000 * 3];
	GLuint framebuffer2;
	glGenFramebuffers(1, &framebuffer2);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);

	// create a color attachment texture

	std::shared_ptr<Texture> textureColor = std::make_shared<Texture>();
	textureColor->CreateTexture(100, 100);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColor->GetTextureID(), 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("no");
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer2);
	glBlitFramebuffer(0, 0, 1024, 768, 0, 0, 100, 100, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
	glReadPixels(0, 0, 100, 100, GL_RGB, GL_UNSIGNED_BYTE, bytes);

	SOIL_save_image("hello.bmp", SOIL_SAVE_TYPE_BMP, 100, 100, 3, bytes);
	delete[] bytes;

	glDeleteFramebuffers(1, &framebuffer);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	mVisiableTexture = std::make_shared<Texture>();
	mVisiableTexture->CreateTexture(1024, 768);

	// �湮 �ؽ��� ����
	mVisitedTexture  = std::make_shared<Texture>();
	mVisitedTexture->CreateTexture(1024, 768);
	
	// Fog �ؽ��� ����
	mFogTexture = std::make_shared<Texture>();
	mFogTexture->CreateTexture(1024, 768);

}

void FogOfWar::Update(float deltaTime, std::vector<std::shared_ptr<Unit>> unit)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	//
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	glm::mat4 proj = glm::ortho(-50.0f, 50.f, -50.0f, 50.0f, -1000.0f, 1000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(50.0f, -100.0f, 50.0f), glm::vec3(50.0f, 0.0f, 50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	// visiable ���� ��ġ
	{
		glBindVertexArray(mVao);
		// �ݺ������� ��ġ ���� �ִ´�.
		GLuint framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mVisiableTexture->GetTextureID(), 0);

		// Frame buffer ������ ����
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			return;
		}

		//������
		std::vector<std::pair<std::string, int> > visiableCodies;
		Shader visiableShader;
		visiableCodies.clear();
		visiableCodies.push_back(make_pair(ReadShaderFile("visiable.vert"), GL_VERTEX_SHADER));
		visiableCodies.push_back(make_pair(ReadShaderFile("visiable.frag"), GL_FRAGMENT_SHADER));
		visiableShader.BuildShader(visiableCodies);
		
		
		std::vector<float> posData(unit.size() * 3);
		for (int i = 0; i < 10; ++i)
		{
			glm::vec3 pos = unit[i]->GetPosition();
			posData[i * 3] = pos.x;
			posData[i * 3 + 1] = pos.y;
			posData[i * 3 + 2] = -pos.z;
		}
		
		GLuint unitPosBuffer;
		glGenBuffers(1, &unitPosBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, unitPosBuffer);
		
		glBufferData(GL_ARRAY_BUFFER, posData.size() * sizeof(float), &posData[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, unitPosBuffer);

		glVertexAttribPointer(
			2,                  // 0��° �Ӽ�(attribute).
			3,                  // ũ��(size)
			GL_FLOAT,           // Ÿ��(type)
			GL_FALSE,           // ����ȭ(normalized)?
			0,                  // ���� ��� ���� ����(stride)
			(void*)0            // �迭 ������ ������(offset; �ű�� ��)
		);

		glVertexAttribDivisor(2, 1);

		// visiable �ؽ��� ����
		static const GLfloat green[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, green);

		glm::mat4 matrix = proj * view;
		visiableShader.SetActive();
		visiableShader.SetMatrixUniform("vp_matrix", matrix);
		
		mSightTexture->SetActive();

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, unit.size());
		
		//PrintScreen(framebuffer, "visiable.bmp");
 		glDeleteFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// visited �湮�� ��ġ �ؽ���
	{
		glBindVertexArray(mVao);
		GLuint framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		static const GLfloat green[] = { 0.0f, 0.0f, 0.0f, 1.0f };

		glClearBufferfv(GL_COLOR, 0, green);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mVisitedTexture->GetTextureID(), 0);

		// Frame buffer ������ ����
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			return;
		}

		//������
		std::vector<std::pair<std::string, int> > visitedCodies;
		Shader visitedShader;

		visitedCodies.clear();
		visitedCodies.push_back(make_pair(ReadShaderFile("visited.vert"), GL_VERTEX_SHADER));
		visitedCodies.push_back(make_pair(ReadShaderFile("visited.frag"), GL_FRAGMENT_SHADER));
		visitedShader.BuildShader(visitedCodies);


		visitedShader.SetActive();
		mVisiableTexture->SetActive();

		// TODO ������ Blend �Լ��� ����
		glEnable(GL_BLEND);
		glBlendEquation(GL_MAX);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		int a = 0;
		if (a == 1)
			PrintScreen(framebuffer, "visited.bmp");

		glDeleteFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	// Fog of War
	{
		glBindVertexArray(mVao);
		GLuint framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFogTexture->GetTextureID(), 0);

		// Frame buffer ������ ����
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			return;
		}

		//������
		std::vector<std::pair<std::string, int> > fogCodies;
		Shader fogShader;

		fogCodies.clear();
		fogCodies.push_back(make_pair(ReadShaderFile("st.vert"), GL_VERTEX_SHADER));
		fogCodies.push_back(make_pair(ReadShaderFile("fog.frag"), GL_FRAGMENT_SHADER));
		fogShader.BuildShader(fogCodies);

		static const GLfloat green[] = { 0.0f, 0.0f, 0.0f, 1.0f };

		glClearBufferfv(GL_COLOR, 0, green);

		fogShader.SetActive();
		glActiveTexture(GL_TEXTURE0);
		mVisiableTexture->SetActive();
		glActiveTexture(GL_TEXTURE1);
		mVisitedTexture->SetActive();

		glBlendEquation(GL_MAX);
		// TODO ������ Blend �Լ��� ����
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		int a = 0;
		if (a == 1)
			PrintScreen(framebuffer, "Fog.bmp");

		glDeleteFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void FogOfWar::Render()
{

}

void FogOfWar::PrintScreen(GLuint framebuffer, const std::string& str)
{
	unsigned char* bytes = new unsigned char[1024 * 1024 * 3];
	GLuint drawFrameBuffer;
	glGenFramebuffers(1, &drawFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, drawFrameBuffer);

	// create a color attachment texture
	std::shared_ptr<Texture> textureColorbuffer2 = std::make_shared<Texture>();
	textureColorbuffer2->CreateTexture(1024, 1024);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer2->GetTextureID(), 0);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("frame buffer ������ ����");
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFrameBuffer);
	glBlitFramebuffer(0, 0, 1024, 768, 0, 0, 1024, 1024, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, drawFrameBuffer);
	glReadPixels(0, 0, 1024, 1024, GL_RGB, GL_UNSIGNED_BYTE, bytes);

	SOIL_save_image(str.c_str(), SOIL_SAVE_TYPE_BMP, 1024, 1024, 3, bytes);
	
	delete[] bytes;
	glDeleteFramebuffers(1, &drawFrameBuffer);
}
