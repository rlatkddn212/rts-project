#include "Precompiled.h"
#include "MiniMap.h"

MiniMap::MiniMap()
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

	// Frame buffer ������ ����
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return;
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// �̴ϸʿ� ȭ�� ��ġ
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

MiniMap::~MiniMap()
{
	glDeleteFramebuffers(1, &mFrameBuffer);
}

void MiniMap::Update(float deltaTime, std::vector<std::shared_ptr<Unit>> unit, std::shared_ptr<Camera> camera)
{
	Ray ray[4];
	ray[0].SetRay(camera, 0, 0);
	ray[1].SetRay(camera, 1024, 0);
	ray[2].SetRay(camera, 1024, 768);
	ray[3].SetRay(camera, 0, 768);

	std::vector<glm::vec3> pos;
	for (int i = 0; i < 4; ++i)
	{
		float t = 0.0f;
		if (RayIntersectPlane(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0, 0.0, 0.0), ray[i].org, ray[i].dir, &t))
		{
			pos.push_back(ray[i].org + ray[i].dir * t);
		}
	}

	std::vector<GLfloat> verts;
	if (pos.size() >= 4)
	{
		glBindVertexArray(mVertexArray);
		unsigned vertexSize = 7 * sizeof(float);

		for (int i = 0; i < 4; ++i)
		{
			verts.push_back((pos[i].x - 50.0f) / 50.0f);
			verts.push_back((-pos[i].z - 50.0f) / 50.0f);
			verts.push_back(pos[i].y);
			verts.push_back(1.0f);
			verts.push_back(1.0f);
			verts.push_back(1.0f);
			verts.push_back(1.0f);
		}

		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, 4 * vertexSize, &verts[0], GL_STATIC_DRAW);
	}


	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
	// visible �ؽ��� ����
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
	mShader->SetMatrixUniform("mMatrix", mat);
	mLineShader->SetMatrixUniform("mMatrix", mat);
	glBindVertexArray(mVertexArray);
	glLineWidth(2.f);
	glEnable(GL_SCISSOR_TEST);
	glScissor(1024.0f * 7.0f / 10.0f, 0.0f, 1024.0f * 3.0f / 10.0f, 768.0f * 3.0f / 10.0f);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	
	glDisable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);
}

bool MiniMap::RayIntersectPlane(glm::vec3 n, glm::vec3 p0, glm::vec3 org, glm::vec3 dir, float* t)
{
 	float denom = glm::dot(n, dir);
	if (fabs(denom) > 1e-6) {
		glm::vec3 p0l0 = p0 - org;
		*t = glm::dot(p0l0, n) / denom;
		return (t >= 0);
	}

	return false;
}
