#include "Precompiled.h"
#include "FogOfWar.h"
#include "Shader.h"

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

	// create a color attachment texture
	glGenTextures(1, &mSightTexture);
	glBindTexture(GL_TEXTURE_2D, mSightTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mSightTexture, 0);

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

	static const GLfloat green[] = { 1.0f, 0.25f, 1.0f, 1.0f };

	glClearBufferfv(GL_COLOR, 0, green);
	glBindVertexArray(mVao);
	sightShader.SetActive();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FogOfWar::Update(float deltaTime)
{
}

void FogOfWar::Render()
{
	// visiable ���� ��ġ
	{
		glBindVertexArray(mVao);

		// �ݺ������� ��ġ ���� �ִ´�.
		GLuint framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glGenTextures(1, &mVisiableTexture);
		glBindTexture(GL_TEXTURE_2D, mVisiableTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mVisiableTexture, 0);

		// Frame buffer ������ ����
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			return;
		}

		//������
		std::vector<std::pair<std::string, int> > visiableCodies;
		Shader visiableShader;

		visiableCodies.clear();
		visiableCodies.push_back(make_pair(ReadShaderFile("st.vert"), GL_VERTEX_SHADER));
		visiableCodies.push_back(make_pair(ReadShaderFile("visiable.frag"), GL_FRAGMENT_SHADER));
		visiableShader.BuildShader(visiableCodies);

		static const GLfloat green[] = { 1.0f, 0.25f, 1.0f, 1.0f };

		glClearBufferfv(GL_COLOR, 0, green);
		glBindVertexArray(mVao);
		
		visiableShader.SetActive();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mSightTexture);

		// �ν��Ͻ�
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	// visited �湮�� ��ġ �ؽ���
	{
		glBindVertexArray(mVao);
		GLuint framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glGenTextures(1, &mVisitedTexture);
		glBindTexture(GL_TEXTURE_2D, mVisitedTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mVisitedTexture, 0);

		// Frame buffer ������ ����
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			return;
		}

		//������
		std::vector<std::pair<std::string, int> > visitedCodies;
		Shader visitedShader;

		visitedCodies.clear();
		visitedCodies.push_back(make_pair(ReadShaderFile("st.vert"), GL_VERTEX_SHADER));
		visitedCodies.push_back(make_pair(ReadShaderFile("visited.frag"), GL_FRAGMENT_SHADER));
		visitedShader.BuildShader(visitedCodies);

		static const GLfloat green[] = { 1.0f, 0.25f, 1.0f, 1.0f };

		glClearBufferfv(GL_COLOR, 0, green);
		glBindVertexArray(mVao);

		visitedShader.SetActive();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mSightTexture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mVisitedTexture);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	// Fog of War
	{
		glBindVertexArray(mVao);
		GLuint framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glGenTextures(1, &mFogTexture);
		glBindTexture(GL_TEXTURE_2D, mFogTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFogTexture, 0);

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

		static const GLfloat green[] = { 1.0f, 0.25f, 1.0f, 1.0f };

		glClearBufferfv(GL_COLOR, 0, green);
		glBindVertexArray(mVao);

		fogShader.SetActive();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mVisiableTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mVisitedTexture);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}
