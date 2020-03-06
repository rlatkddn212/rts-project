#include "Precompiled.h"
#include "FogOfWar.h"
#include "Shader.h"
#include <vector>

FogOfWar::FogOfWar(int w, int h)
{
	mWidth = w;
	mHeight = h;

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
	glVertexAttribPointer(0, 3,	GL_FLOAT, GL_FALSE,	0, (void*)0);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvBufferData), uvBufferData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	mSightTexture = std::make_shared<Texture>();
	mSightTexture->CreateTexture(mWidth, mHeight, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mSightTexture->GetTextureID(), 0);

	// Frame buffer 완전성 위배
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		assert(0);
		return;
	}

	//랜더링
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

	glDeleteFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	mVisibleTexture = std::make_shared<Texture>();
	mVisibleTexture->CreateTexture(mWidth, mHeight, 0);

	// 방문 텍스쳐 생성
	mVisitedTexture  = std::make_shared<Texture>();
	mVisitedTexture->CreateTexture(mWidth, mHeight, 0);
	
	// Fog 텍스쳐 생성
	mFogTexture = std::make_shared<Texture>();
	mFogTexture->CreateTexture(mWidth, mHeight, 0);

	// 유닛 시야
	std::vector<std::pair<std::string, int> > visibleCodies;
	
	visibleCodies.clear();
	visibleCodies.push_back(make_pair(ReadShaderFile("visible.vert"), GL_VERTEX_SHADER));
	visibleCodies.push_back(make_pair(ReadShaderFile("visible.frag"), GL_FRAGMENT_SHADER));
	mVisibleShader.BuildShader(visibleCodies);

	// 방문
	std::vector<std::pair<std::string, int> > visitedCodies;

	visitedCodies.clear();
	visitedCodies.push_back(make_pair(ReadShaderFile("visited.vert"), GL_VERTEX_SHADER));
	visitedCodies.push_back(make_pair(ReadShaderFile("visited.frag"), GL_FRAGMENT_SHADER));
	mVisitedShader.BuildShader(visitedCodies);

	// 안개 텍스쳐
	std::vector<std::pair<std::string, int> > fogCodies;

	fogCodies.clear();
	fogCodies.push_back(make_pair(ReadShaderFile("st.vert"), GL_VERTEX_SHADER));
	fogCodies.push_back(make_pair(ReadShaderFile("fog.frag"), GL_FRAGMENT_SHADER));
	mFogShader.BuildShader(fogCodies);
}

void FogOfWar::MakeFogTexture(const std::vector<std::shared_ptr<RTSObject> >& renderobj)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glm::mat4 proj = glm::ortho(-50.0f, 50.f, -50.0f, 50.0f, -1000.0f, 1000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(50.0f, -100.0f, 50.0f), glm::vec3(50.0f, 0.0f, 50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	
	// visible 유닛 위치
	{
		glBindVertexArray(mVao);
		// 반복문으로 위치 값을 넣는다.
		GLuint framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mVisibleTexture->GetTextureID(), 0);

		// Frame buffer 완전성 위배
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			return;
		}

		std::vector<float> posData(renderobj.size() * 3);
		for (size_t i = 0; i < renderobj.size(); ++i)
		{
			glm::vec3 pos = renderobj[i]->GetPosition();
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

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glVertexAttribDivisor(2, 1);

		// visible 텍스쳐 생성
		static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);

		glm::mat4 matrix = proj * view;
		mVisibleShader.SetActive();
		mVisibleShader.SetMatrixUniform("vp_matrix", matrix);
		
		mSightTexture->SetActive();

		glBlendFunc(GL_ONE, GL_ONE);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, renderobj.size());
		
		//PrintScreen(framebuffer, "visiable.bmp");
		glDeleteBuffers(1, &unitPosBuffer);
 		glDeleteFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// visited 방문한 위치 텍스쳐
	{
		glBindVertexArray(mVao);
		GLuint framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mVisitedTexture->GetTextureID(), 0);

		// Frame buffer 완전성 위배
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			return;
		}

		mVisitedShader.SetActive();
		mVisibleTexture->SetActive();

		glBlendEquation(GL_MAX);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		//PrintScreen(framebuffer, "visited.bmp");
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

		// Frame buffer 완전성 위배
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			return;
		}

		static const GLfloat black[] = { 0.1f, 0.1f, 0.1f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);

		mFogShader.SetActive();
		glActiveTexture(GL_TEXTURE0);
		mVisibleTexture->SetActive();
		glActiveTexture(GL_TEXTURE1);
		mVisitedTexture->SetActive();

		glBlendEquation(GL_MAX);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		//PrintScreen(framebuffer, "Fog.bmp");

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
	// 1024는 임의의 크기로 수정 가능
	unsigned char* bytes = new unsigned char[1024 * 1024 * 3];
	GLuint drawFrameBuffer;
	glGenFramebuffers(1, &drawFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, drawFrameBuffer);

	// create a color attachment texture
	std::shared_ptr<Texture> textureColorbuffer2 = std::make_shared<Texture>();
	textureColorbuffer2->CreateTexture(1024, 1024, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer2->GetTextureID(), 0);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("frame buffer 완전성 위배");
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFrameBuffer);
	glBlitFramebuffer(0, 0, mWidth, mHeight, 0, 0, 1024, 1024, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, drawFrameBuffer);
	glReadPixels(0, 0, mWidth, mHeight, GL_RGB, GL_UNSIGNED_BYTE, bytes);

	SOIL_save_image(str.c_str(), SOIL_SAVE_TYPE_BMP, 1024, 1024, 3, bytes);
	
	delete[] bytes;
	glDeleteFramebuffers(1, &drawFrameBuffer);
}
