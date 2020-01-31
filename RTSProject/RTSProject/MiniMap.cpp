#include "Precompiled.h"
#include "MiniMap.h"
#include "ObjectResourcePool.h"

MiniMap::MiniMap(int winSizeX, int winSizeY, int miniX, int miniY)
{
	mRenderState = UIRendering;
	mWinX = winSizeX;
	mWinY = winSizeY;
	mMiniMapX = miniX;
	mMiniMapY = miniY;

	std::shared_ptr<MiniMap> mini = ObjectResourcePool::GetInstance()->GetMiniMap();
	mVao = mini->GetVao();
	mVertexArray = mini->GetVertexArray();
	mVertexBuffer = mini->GetVertexBuffer();

	mFogTexture = mini->GetFogTexture();
	mPositionTexture = mini->GetPositionTexture();
	mUnitTexture = mini->GetUnitTexture();
	mMapTexture = mini->GetMapTexture();

	mLineShader = mini->GetLineShader();
	mShader = mini->GetShader();
}

MiniMap::~MiniMap()
{
	glDeleteFramebuffers(1, &mFrameBuffer);
}

void MiniMap::MakeModel()
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvBufferData), uvBufferData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	std::vector<std::pair<std::string, int> > shaderCodies;
	shaderCodies.push_back(make_pair(ReadShaderFile("minimap.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("minimap.frag"), GL_FRAGMENT_SHADER));
	mShader = std::make_shared<Shader>();
	mShader->BuildShader(shaderCodies);

	mPositionTexture = std::make_shared<Texture>();
	mPositionTexture->CreateTexture(100, 100, 0);

	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 미니맵에 화면 위치
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	unsigned vertexSize = 7 * sizeof(float);

	float verts[] =
	{
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		0.0f, 0.5f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		0.5f, 0.5f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		0.5f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
	};

	// Create vertex buffer
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * vertexSize, verts, GL_STATIC_DRAW);

	// Position is 3 floats
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

	// Color is 4 floats
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize,
		reinterpret_cast<void*>(sizeof(float) * 3));

	shaderCodies.clear();
	shaderCodies.push_back(make_pair(ReadShaderFile("line.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("color.frag"), GL_FRAGMENT_SHADER));
	mLineShader = std::make_shared<Shader>();
	mLineShader->BuildShader(shaderCodies);
}

void MiniMap::UpdateModel()
{
	std::vector<GLfloat> verts;
	if (mPos.size() >= 4)
	{
		glBindVertexArray(mVertexArray);
		unsigned vertexSize = 7 * sizeof(float);

		for (size_t i = 0; i < 4; ++i)
		{
			verts.push_back((mPos[i].x - 50.0f) / 50.0f);
			verts.push_back((-mPos[i].z - 50.0f) / 50.0f);
			verts.push_back(mPos[i].y);
			verts.push_back(1.0f);
			verts.push_back(1.0f);
			verts.push_back(1.0f);
			verts.push_back(1.0f);
		}

		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, 4 * vertexSize, &verts[0], GL_STATIC_DRAW);
	}

	glGenFramebuffers(1, &mFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPositionTexture->GetTextureID(), 0);

	// Frame buffer 완전성 위배
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return;
	}

	// visible 텍스쳐 생성
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat green[] = { 0.0f, 1.0f, 0.0f, 1.0f };

	glClearBufferfv(GL_COLOR, 0, black);
	glEnable(GL_SCISSOR_TEST);
	for (size_t unitIdx = 0; unitIdx < mUnit.size(); ++unitIdx)
	{
		glm::vec3 pos = mUnit[unitIdx]->GetPosition();
		glm::vec2 screen = glm::vec2(pos.x, -pos.z);

		glScissor(screen.x, screen.y, 1, 1);
		glClearBufferfv(GL_COLOR, 0, green);
	}

	glDisable(GL_SCISSOR_TEST);

	glDeleteFramebuffers(1, &mFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MiniMap::Update(float deltaTime, const std::vector<std::shared_ptr<Unit>>& unit, std::shared_ptr<Camera> camera)
{
	mUnit = unit;

	Ray ray[4];
	ray[0].SetRay(camera, 0, 0);
	ray[1].SetRay(camera, mWinX, 0);
	ray[2].SetRay(camera, mWinX, mWinY);
	ray[3].SetRay(camera, 0, mWinY);

	mPos.clear();
	mPos.reserve(4);
	for (size_t i = 0; i < 4; ++i)
	{
		float t = 0.0f;
		if (RayIntersectPlane(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0, 0.0, 0.0), ray[i].org, ray[i].dir, &t))
		{
			mPos.push_back(ray[i].org + ray[i].dir * t);
		}
	}
}

void MiniMap::Render(std::shared_ptr<Camera> camera)
{
	glDisable(GL_DEPTH_TEST);

	mShader->SetActive();
	
	glm::mat4 mat = glm::translate(glm::mat4(1.0f), glm::vec3(1.0 - 0.3, 0.3 -1.0, 0.0))
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.3, 0.3, 0.3));
	mShader->SetMatrixUniform("mMatrix", mat);

	glBindVertexArray(mVao);
	glActiveTexture(GL_TEXTURE0);
	mMapTexture->SetActive();
	glActiveTexture(GL_TEXTURE1);
	mFogTexture->SetActive();
	glActiveTexture(GL_TEXTURE2);
	mPositionTexture->SetActive();
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glActiveTexture(GL_TEXTURE0);

	mLineShader->SetActive();
	mLineShader->SetMatrixUniform("mMatrix", mat);
	glBindVertexArray(mVertexArray);
	glLineWidth(2.f);
	glEnable(GL_SCISSOR_TEST);
	glScissor(mWinX - mMiniMapX, 0.0f, mMiniMapX, mMiniMapY);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	
	glDisable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);
}

void MiniMap::AddRender(std::shared_ptr<Camera> camera)
{
	std::shared_ptr<MiniMap> ro = std::make_shared<MiniMap>(*this);
	ro->mCamera = camera;
	RenderManager::GetInstance()->AddQueue(ro);
}

bool MiniMap::RayIntersectPlane(glm::vec3 n, glm::vec3 p0, glm::vec3 org, glm::vec3 dir, float* t)
{
 	float denom = glm::dot(n, dir);
	if (fabs(denom) > 1e-6) 
	{
		glm::vec3 p0l0 = p0 - org;
		*t = glm::dot(p0l0, n) / denom;
		return (t >= 0);
	}

	return false;
}

bool MiniMap::IsXYInMiniMap(int mouseX, int mouseY)
{
	 
	if (mWinX - mMiniMapX < mouseX && mouseX < mWinX
		&& mWinY - mMiniMapY < mouseY && mouseY < mWinY)
	{
		return true;
	}

	return false;
}

glm::vec2 MiniMap::GetTerrainPos(int mouseX, int mouseY)
{
	// terrain size가 100인것을 가정한다
	float posX = (float)(mouseX - (mWinX - mMiniMapX)) / (float)mMiniMapX * 100.0f;
	float posY = (float)(mouseY - (mWinY - mMiniMapY)) / (float)mMiniMapY * 100.0f;

	return glm::vec2(posX, - (100.0f - posY));
}
