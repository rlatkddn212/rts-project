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

	static const GLfloat green[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glClearBufferfv(GL_COLOR, 0, green);
	sightShader.SetActive();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	unsigned char* bytes = new unsigned char[10000 * 3];
	unsigned int framebuffer2;
	glGenFramebuffers(1, &framebuffer2);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);

	// create a color attachment texture
	unsigned int textureColorbuffer2;
	glGenTextures(1, &textureColorbuffer2);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 100, 100, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer2, 0);

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
}


// 
void FogOfWar::Update(float deltaTime, std::vector<std::shared_ptr<Unit>> unit)
{

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	//
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	glm::mat4 proj = glm::ortho(-100.f, 100.f, -100.0f, 100.0f, -1000.0f, 1000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(50.0f, 100.0f, -50.0f), glm::vec3(50.0f, 0.0f, -50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
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
		visiableCodies.push_back(make_pair(ReadShaderFile("visiable.vert"), GL_VERTEX_SHADER));
		visiableCodies.push_back(make_pair(ReadShaderFile("visiable.frag"), GL_FRAGMENT_SHADER));
		visiableShader.BuildShader(visiableCodies);
		
		
		std::vector<float> posData(unit.size() * 3);
		for (int i = 0; i < 10; ++i)
		{
			glm::vec3 pos = unit[i]->GetPosition();
			posData[i * 3] = pos.x;
			posData[i * 3 + 1] = pos.y;
			posData[i * 3 + 2] = pos.z;
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
		
		glBindTexture(GL_TEXTURE_2D, mSightTexture);

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 10);
		
		unsigned char* bytes = new unsigned char[1024 * 1024 * 3];
		unsigned int framebuffer2;
		glGenFramebuffers(1, &framebuffer2);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);

		// create a color attachment texture
		unsigned int textureColorbuffer2;
		glGenTextures(1, &textureColorbuffer2);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer2, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("no");
		}

		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer2);
		glBlitFramebuffer(0, 0, 1024, 768, 0, 0, 1024, 1024, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
		glReadPixels(0, 0, 1024, 1024, GL_RGB, GL_UNSIGNED_BYTE, bytes);

		//SOIL_save_image("visiable.bmp", SOIL_SAVE_TYPE_BMP, 1024, 1024, 3, bytes);
		delete[] bytes;
		
		glDeleteFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	{

	/*
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
		
		GLuint VertexArrayID;
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		static const GLfloat g_vertex_buffer_data[] =
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
			0.5f,  0.5f, 0.0f,
		};
		static const GLfloat g_uv_buffer_data[] =
		{
			0.0, 1.0,
			1.0, 1.0,
			0.0, 0.0,
			1.0, 0.0,
		};

		// ���ؽ� ���ۿ� �ڵ�
		GLuint vertexbuffer[2];
		// ���۸� ����
		glGenBuffers(2, &vertexbuffer[0]);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
		// ���ؽ����� OpenGL��
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
		// ���ؽ����� OpenGL��
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

		static const GLfloat posData[] =
		{
			0.0f, -0.0f, -0.0f,
		};

		GLuint unitPosBuffer;
		glGenBuffers(1, &unitPosBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, unitPosBuffer);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3, &posData[0], GL_STATIC_DRAW);

		//������
		std::vector<std::pair<std::string, int> > visiableCodies;
		Shader visiableShader;

		visiableCodies.clear();
		visiableCodies.push_back(make_pair(ReadShaderFile("visiable.vert"), GL_VERTEX_SHADER));
		visiableCodies.push_back(make_pair(ReadShaderFile("visiable.frag"), GL_FRAGMENT_SHADER));
		visiableShader.BuildShader(visiableCodies);

		static const GLfloat blue[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, blue);

		glm::mat4 matrix = proj * view;
		visiableShader.SetMatrixUniform("vp_matrix", matrix);

		glBindTexture(GL_TEXTURE_2D, mSightTexture);
		visiableShader.SetActive();

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);

		glVertexAttribPointer(
			0,                  // 0��° �Ӽ�(attribute).
			3,                  // ũ��(size)
			GL_FLOAT,           // Ÿ��(type)
			GL_FALSE,           // ����ȭ(normalized)?
			0,                  // ���� ��� ���� ����(stride)
			(void*)0            // �迭 ������ ������(offset; �ű�� ��)
		);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);

		glVertexAttribPointer(
			1,                  // 0��° �Ӽ�(attribute).
			2,                  // ũ��(size)
			GL_FLOAT,           // Ÿ��(type)
			GL_FALSE,           // ����ȭ(normalized)?
			0,                  // ���� ��� ���� ����(stride)
			(void*)0            // �迭 ������ ������(offset; �ű�� ��)
		);

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
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);c
		*/
		/*
		unsigned char* bytes = new unsigned char[10000 * 3];
		unsigned int framebuffer2;
		glGenFramebuffers(1, &framebuffer2);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);

		// create a color attachment texture
		unsigned int textureColorbuffer2;
		glGenTextures(1, &textureColorbuffer2);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 100, 100, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer2, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("no");
		}

		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer2);
		glBlitFramebuffer(0, 0, 1024, 768, 0, 0, 100, 100, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
		glReadPixels(0, 0, 100, 100, GL_RGB, GL_UNSIGNED_BYTE, bytes);

		SOIL_save_image("visiable2.bmp", SOIL_SAVE_TYPE_BMP, 100, 100, 3, bytes);
		delete[] bytes;

		glDeleteFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		*/
	}


	/*
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
	*/
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

}

void FogOfWar::Render()
{

}
