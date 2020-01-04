#include "Precompiled.h"
#include "Mouse.h"

Mouse::Mouse(GLFWwindow* window)
{
	isVisiableDragBox = false;
	lastWheel = 0.0;
	InitMouse(window);
}

Mouse::~Mouse()
{
}

void Mouse::InitMouse(GLFWwindow* window)
{
	int channels = 0;
	int mHeight;
	int mWidth;
	unsigned char* image2 = SOIL_load_image("Cursor/cursor.dds", &mWidth, &mHeight, &channels, SOIL_LOAD_AUTO);

	unsigned char pixel[20 * 20 * 4];
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			pixel[i * 20 * channels + j * channels + 0] = image2[i * mWidth  * channels + j * channels + 0];
			pixel[i * 20 * channels + j * channels + 1] = image2[i * mWidth  * channels + j * channels + 1];
			pixel[i * 20 * channels + j * channels + 2] = image2[i * mWidth  * channels + j * channels + 2];
			pixel[i * 20 * channels + j * channels + 3] = image2[i * mWidth  * channels + j * channels + 3];
		}
	}

	GLFWimage image;
	image.width = 20;
	image.height = 20;
	image.pixels = pixel;
	cursor = glfwCreateCursor(&image, 0, 0);
	glfwSetCursor(window, cursor);

	glfwGetWindowSize(window, &mW, &mH);
	glfwSetCursorPos(window, mW / 2, mH / 2);
	mX = mW / 2;
	mY = mH / 2;

	SOIL_free_image_data(image2);

	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	unsigned vertexSize = 7 * sizeof(float);

	float verts[] =
	{
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		
		0.0f, 0.1f, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		0.1f, 0.1f, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		
		0.1f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
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

	std::vector<std::pair<std::string, int> > shaderCodies;
	shaderCodies.push_back(make_pair(ReadShaderFile("line.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("color.frag"), GL_FRAGMENT_SHADER));
	mMeshShader = std::make_shared<Shader>();
	mMeshShader->BuildShader(shaderCodies);
}

void Mouse::SetEndXY(double x, double y)
{
	mX = x;
	mY = y;

	glBindVertexArray(mVertexArray);
	unsigned vertexSize = 7 * sizeof(float);

	float verts[] =
	{
		-1.0f + 2 * mPrevX / mW, 1.0f - 2 * mPrevY / mH, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		-1.0f + 2 * mPrevX / mW, 1.0f - 2 * mY / mH, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		-1.0f + 2 * mX / mW, 1.0f - 2 * mY / mH, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		-1.0f + 2 * mX / mW, 1.0f - 2 * mPrevY / mH, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * vertexSize, verts, GL_STATIC_DRAW);
}

void Mouse::SetStartXY(double x, double y)
{
	mPrevX = x;
	mPrevY = y;
}

void Mouse::Render(std::shared_ptr<Camera> camera)
{
	if (isVisiableDragBox)
	{
		mMeshShader->SetActive();
		mMeshShader->SetMatrixUniform("mMatrix", glm::mat4(1.0f));
		glBindVertexArray(mVertexArray);
		glLineWidth(2.f);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
	}
}

bool Mouse::IsDragBoxPos(glm::vec2 p)
{
	float minX = glm::min(mPrevX, mX);
	float minY = glm::min(mPrevY, mY);

	float maxX = glm::max(mPrevX, mX);
	float maxY = glm::max(mPrevY, mY);

	if ((minX < p.x && p.x < maxX) && (minY < p.y && p.y < maxY))
	{
		return true;
	}

	return false;
}