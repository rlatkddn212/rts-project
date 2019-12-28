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

	// Frame buffer 완전성 위배
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return;
	}

	//랜더링
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
	// visiable 유닛 위치
	{
		glBindVertexArray(mVao);

		// 반복문으로 위치 값을 넣는다.
		GLuint framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glGenTextures(1, &mVisiableTexture);
		glBindTexture(GL_TEXTURE_2D, mVisiableTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mVisiableTexture, 0);

		// Frame buffer 완전성 위배
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			return;
		}

		//랜더링
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

		// 인스턴스
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	// visited 방문한 위치 텍스쳐
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

		// Frame buffer 완전성 위배
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			return;
		}

		//랜더링
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

		// Frame buffer 완전성 위배
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			return;
		}

		//랜더링
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
