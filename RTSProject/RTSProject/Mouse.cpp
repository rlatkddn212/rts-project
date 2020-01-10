#include "Precompiled.h"
#include "Mouse.h"

Mouse::Mouse(GLFWwindow* window)
{
	mWindow = window;
	isVisiableDragBox = false;
	lastWheel = 0.0;
	InitMouse();
}

Mouse::~Mouse()
{
}

void Mouse::InitMouse()
{
	UseGLFWCursor();
	UseRenderCursor();
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

void Mouse::SetCursorImage(int type)
{
	GLFWimage image;
	image.width = 20;
	image.height = 20;
	image.pixels = mPixel[type];
	cursor = glfwCreateCursor(&image, 0, 0);
	glfwSetCursor(mWindow, cursor);

	switch (type)
	{
	case 0:
		mCursorUI->SetUV(0.0f, 0.0f, 20.0f / 64.0f, 20.0f / 64.0f);
		break;
	case 1:
		mCursorUI->SetUV(0.0f, 20.0f / 64.0f, 20.0f / 64.0f, 40.0f / 64.0f);
		break;
	case 2:
		mCursorUI->SetUV(20.0f / 64.0f, 20.0f / 64.0f, 40.0f / 64.0f, 40.0f / 64.0f);
		break;
	case 3:
		mCursorUI->SetUV(0.0f, 40.0f / 64.0f, 20.0f / 64.0f, 60.0f / 64.0f);
		break;
	case 4:
		mCursorUI->SetUV(20.0f / 64.0f, 40.0f / 64.0f, 40.0f / 64.0f, 60.0f / 64.0f);
		break;
	default:
		mCursorUI->SetUV(0.0f, 0.0f, 20.0f / 64.0f, 20.0f / 64.0f);
		break;
	}
}

void Mouse::SetEndXY(double x, double y)
{
	mX = x;
	mY = y;

	glBindVertexArray(mVertexArray);
	unsigned vertexSize = 7 * sizeof(float);

	float verts[] =
	{
		-1.0f + 2 * mPrevX / mWidth, 1.0f - 2 * mPrevY / mHeight, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		-1.0f + 2 * mPrevX / mWidth, 1.0f - 2 * mY / mHeight, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		-1.0f + 2 * mX / mWidth, 1.0f - 2 * mY / mHeight, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		-1.0f + 2 * mX / mWidth, 1.0f - 2 * mPrevY / mHeight, 0.0f,
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
	mCursorUI->Render(camera);

	if (isVisiableDragBox)
	{
		glDisable(GL_DEPTH_TEST);
		mMeshShader->SetActive();
		mMeshShader->SetMatrixUniform("mMatrix", glm::mat4(1.0f));
		glBindVertexArray(mVertexArray);
		glLineWidth(2.f);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
		glEnable(GL_DEPTH_TEST);
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

void Mouse::UseGLFWCursor()
{
	int channels = 0;
	int height;
	int width;
	unsigned char* cursorImage = SOIL_load_image("Cursor/cursor.dds", &width, &height, &channels, SOIL_LOAD_AUTO);

	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			mPixel[0][i * 20 * channels + j * channels + 0] = cursorImage[i * width * channels + j * channels + 0];
			mPixel[0][i * 20 * channels + j * channels + 1] = cursorImage[i * width * channels + j * channels + 1];
			mPixel[0][i * 20 * channels + j * channels + 2] = cursorImage[i * width * channels + j * channels + 2];
			mPixel[0][i * 20 * channels + j * channels + 3] = cursorImage[i * width * channels + j * channels + 3];
		}
	}

	for (int k = 0; k < 4; ++k)
	{
		int x = k / 2;
		int y = k % 2 + 1;

		for (int i = 0; i < 20; ++i)
		{
			for (int j = 0; j < 20; ++j)
			{
				mPixel[k + 1][i * 20 * channels + j * channels + 0] = cursorImage[(i + 20 * y) * width  * channels + j * channels + 0 + channels * 20 * x];
				mPixel[k + 1][i * 20 * channels + j * channels + 1] = cursorImage[(i + 20 * y) * width  * channels + j * channels + 1 + channels * 20 * x];
				mPixel[k + 1][i * 20 * channels + j * channels + 2] = cursorImage[(i + 20 * y) * width  * channels + j * channels + 2 + channels * 20 * x];
				mPixel[k + 1][i * 20 * channels + j * channels + 3] = cursorImage[(i + 20 * y) * width  * channels + j * channels + 3 + channels * 20 * x];
			}
		}
	}

	GLFWimage image;
	image.width = 20;
	image.height = 20;
	image.pixels = mPixel[0];
	cursor = glfwCreateCursor(&image, 0, 0);
	glfwSetCursor(mWindow, cursor);

	glfwGetWindowSize(mWindow, &mWidth, &mHeight);
	glfwSetCursorPos(mWindow, mWidth / 2, mHeight / 2);
	mX = mWidth / 2;
	mY = mHeight / 2;

	SOIL_free_image_data(cursorImage);
}

void Mouse::UseRenderCursor()
{
	mCursorUI = std::make_shared<SpriteUI>();
	mCursorUI->SetTexture("Cursor/cursor.dds");
	
	mCursorUI->SetScale(glm::vec2(0.2f, 0.2f));
	mCursorUI->SetUV(0.0f, 0.0f, 20.0f/ 64.0f, 20.0f / 64.0f);
}

void Mouse::SetCursorPos(glm::vec2 pos)
{
	mCursorUI->SetPos(pos);
}
