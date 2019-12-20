#include "Precompiled.h"
#include "HeightMap.h"
#include <SOIL/SOIL.h>

float Noise(int x)
{
	x = (x << 13) ^ x;
	return (1.0f - ((x * (x*x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

float CosInterpolate(float v1, float v2, float a)
{
	float angle = a * M_PI;
	float prc = (1.0f - cos(angle)) * 0.5f;
	return  v1 * (1.0f - prc) + v2 * prc;
}

HeightMap::HeightMap(glm::ivec2 size)
{
	mAngle = 0.0f;
	mSize = size;
	//Reset the heightMap to 0
	mMaxHeight = 20.0f;
	mHeightMap = new float[mSize.x * mSize.y];
	memset(mHeightMap, 0, sizeof(float) * mSize.x * mSize.y);
	
	mHeightMapTexture = nullptr;
}


HeightMap::~HeightMap()
{
	if (mHeightMap != nullptr)
	{
		delete[] mHeightMap;
		mHeightMap = 0;
	}
}

bool HeightMap::LoadFromFile(const std::string& fileName)
{
	memset(mHeightMap, 0, sizeof(float) * mSize.x * mSize.y);
	if (mHeightMapTexture != nullptr)
	{
		mHeightMapTexture->Unload();
	}

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

	return true;
}

// 해설좀 보자
bool HeightMap::CreateRandomHeightMap(int seed, float noiseSize, float persistence, int octaves)
{
	for (int y = 0; y < mSize.y; y++)
	{
		for (int x = 0; x < mSize.x; x++)
		{
			//Scale x & y to the range of 0.0 - m_size
			float xf = ((float)x / (float)mSize.x) * noiseSize;
			float yf = ((float)y / (float)mSize.y) * noiseSize;

			float total = 0;

			for (int i = 0; i < octaves; i++)
			{
				float freq = pow(2.0f, i);
				float amp = pow(persistence, i);

				float tx = xf * freq;
				float ty = yf * freq;
				int tx_int = (int)tx;
				int ty_int = (int)ty;

				float fracX = tx - tx_int;
				float fracY = ty - ty_int;

				float v1 = Noise(tx_int + ty_int * 57 + seed);
				float v2 = Noise(tx_int + 1 + ty_int * 57 + seed);
				float v3 = Noise(tx_int + (ty_int + 1) * 57 + seed);
				float v4 = Noise(tx_int + 1 + (ty_int + 1) * 57 + seed);

				//Smooth in the X-axis
				float i1 = CosInterpolate(v1, v2, fracX);
				float i2 = CosInterpolate(v3, v4, fracX);

				//Smooth in the Y-axis
				total += CosInterpolate(i1, i2, fracY) * amp;
			}

			int b = (int)(128 + total * 128.0f);
			if (b < 0)b = 0;
			if (b > 255)b = 255;

			//Save to heightMap
			mHeightMap[x + y * mSize.x] = ((float)b / 255.0f) * mMaxHeight;
		}
	}

	return true;
}

void HeightMap::CreateParticles()
{

	float* vertices = new float[3 * mSize.x * mSize.y];

	for (int i = 0; i < mSize.y; ++i)
	{
		for (int j = 0; j < mSize.x; ++j)
		{
			vertices[i * mSize.x * 3 + j * 3 + 0] = (float)j;
			vertices[i * mSize.x * 3 + j * 3 + 1] = mHeightMap[j + i * mSize.x];
			vertices[i * mSize.x * 3 + j * 3 + 2] = (float) - i;
		}
	}

	glGenVertexArrays(1, &mVertexArrayID);
	glBindVertexArray(mVertexArrayID);
	// 버퍼를 생성
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	// 버텍스들을 OpenGL로
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mSize.x * mSize.y, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // 0번째 속성(attribute).
		3,                  // 크기(size)
		GL_FLOAT,           // 타입(type)
		GL_FALSE,           // 정규화(normalized)?
		0,                  // 다음 요소 까지 간격(stride)
		(void*)0            // 배열 버퍼의 오프셋(offset; 옮기는 값)
	);

	// 색상을 변경 시킨다.
	GLfloat color[] = { 1.0f, 0.5f, 0.0, 1.0 };
	glVertexAttrib4fv(1, color);

	std::vector<std::pair<std::string, int> > shaderCodies;
	mShader = std::make_shared<Shader>();

	shaderCodies.clear();
	shaderCodies.push_back(make_pair(ReadShaderFile("height.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("height.frag"), GL_FRAGMENT_SHADER));
	mShader->BuildShader(shaderCodies);

	delete[] vertices;
}

void HeightMap::Update(float deltaTime)
{
	mAngle += deltaTime * 0.5f;

	float aspect = (float)1024 / (float)768;
	mPerspect = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 1000.0f);
	glm::vec2 centre = GetCentre();
	mView = glm::lookAt(glm::vec3(centre.x + cos(mAngle) * centre.x * 1.5f, mMaxHeight * 5.0f, -centre.y + sin(mAngle) * centre.y * 1.5f),
		glm::vec3(centre.x, 0.0f, -centre.y),
		glm::vec3(0.0, 1.0, 0.0));
}

void HeightMap::Render(std::shared_ptr<Camera> camera)
{
	glBindVertexArray(mVertexArrayID);

	glPointSize(2);
	mShader->SetActive();

	glm::mat4 mvpMat = glm::mat4(1.0f);
	mvpMat = mPerspect * mView * mvpMat;
	mShader->SetMatrixUniform("mvp_matrix", mvpMat);

	glDrawArrays(GL_POINTS, 0, mSize.x * mSize.y);
}
