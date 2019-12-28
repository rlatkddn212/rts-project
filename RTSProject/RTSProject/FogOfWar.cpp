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
	/*
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
	
	GLuint mVertexbuffer;
	// 버퍼를 생성
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

	unsigned char* image = SOIL_load_image(fileName.c_str(),
		&mWidth, &mHeight, &channels, SOIL_LOAD_AUTO);


	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	unsigned char* bytes = new unsigned char[1024 * 768 * 3];

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
	shaderCodies2.push_back(make_pair(ReadShaderFile("st.frag"), GL_FRAGMENT_SHADER));
	shader2.BuildShader(shaderCodies2);

	static const GLfloat green[] = { 1.0f, 0.25f, 1.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, green);
	glBindVertexArray(vertexArray2);
	shader2.SetActive();
	glBindTexture(GL_TEXTURE_2D, mTexture);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	for (int i = 0; i < mSize.y; ++i)
	{
		for (int j = 0; j < mSize.x; ++j)
		{
			unsigned char *b = bytes + i * mSize.x * 3 + j * 3;
			mHeightMap[j + i * mSize.x] = ((float)*b / 255.0f) * 15.0f;
		}
	}

	delete[] bytes;


	if (m_pVisibleTexture == NULL || m_pVisitedTexture == NULL)return;

	//Set orthogonal rendering view & projection
	m_terrain.SetOrthogonalView();

	//Retrieve the surface of the back buffer
	IDirect3DSurface9 *backSurface = NULL;
	m_pDevice->GetRenderTarget(0, &backSurface);

	//Render the Visible Texture here...
	{
		//Set texture stages and Renderstates
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_ADD);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);

		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);

		//Get the surface of the m_pVisibleTexture
		IDirect3DSurface9 *visibleSurface = NULL;
		m_pVisibleTexture->GetSurfaceLevel(0, &visibleSurface);

		//Set render target to the visible surface
		m_pDevice->SetRenderTarget(0, visibleSurface);

		//Clear render target to black
		if (m_fogOverride)m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff333333, 1.0f, 0);
		else m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

		m_pDevice->BeginScene();

		//Render the sightTexture for all map objects in thisPlayer.
		if (m_thisPlayer < (int)m_players.size() && m_players[m_thisPlayer] != NULL)
			for (int u = 0; u < (int)m_players[m_thisPlayer]->m_mapObjects.size(); u++)
				if (m_players[m_thisPlayer]->m_mapObjects[u] != NULL)
					m_players[m_thisPlayer]->m_mapObjects[u]->RenderSightMesh();

		m_pDevice->EndScene();

		//Restore renderstates etc.
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

		//Release visible surface
		visibleSurface->Release();
	}

	//Render the Visited Texture here...
	{
		IDirect3DSurface9 *visitedSurface = NULL;
		m_pVisitedTexture->GetSurfaceLevel(0, &visitedSurface);

		//Render to the visted texture
		m_pDevice->SetRenderTarget(0, visitedSurface);
		if (m_firstFogOfWar)
		{
			m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
			m_firstFogOfWar = false;
		}

		if (!m_fogOverride)
		{
			m_pDevice->BeginScene();

			m_pDevice->SetTexture(0, m_pVisibleTexture);
			m_pDevice->SetTexture(1, m_pVisitedTexture);

			m_pSprite->Begin(0);
			m_visitedShader.Begin();
			m_pSprite->Draw(m_pVisibleTexture, NULL, NULL, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0xffffffff);
			m_pSprite->End();

			m_visitedShader.End();
			m_pDevice->EndScene();
		}

		//Release visited surface
		visitedSurface->Release();
	}

	//Render the final FogOfWarTexture
	{
		//Get and set surface of the FogOfWarTexture...
		IDirect3DSurface9 *FogOfWarSurface = NULL;
		m_terrain.m_pFogOfWarTexture->GetSurfaceLevel(0, &FogOfWarSurface);
		m_pDevice->SetRenderTarget(0, FogOfWarSurface);

		m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		m_pDevice->BeginScene();

		//Set Textures
		m_pDevice->SetTexture(0, m_pVisibleTexture);
		m_pDevice->SetTexture(1, m_pVisitedTexture);
		m_pDevice->SetTexture(2, m_terrain.m_pLightMap);

		//Draw to the Fog-of-War texture
		m_pSprite->Begin(0);
		m_FogOfWarShader.Begin();
		m_pSprite->Draw(m_pVisibleTexture, NULL, NULL, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0xffffffff);
		m_pSprite->End();

		m_FogOfWarShader.End();
		m_pDevice->EndScene();

		//Release fog of war surface
		FogOfWarSurface->Release();
	}

	//Reset render target to back buffer
	m_pDevice->SetRenderTarget(0, backSurface);
	backSurface->Release();
	*/
}
