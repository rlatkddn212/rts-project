#include "Precompiled.h"
#include "Mouse.h"

Mouse::Mouse(GLFWwindow* window)
{
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
}


void Mouse::Update(GLfloat x, GLfloat y, double wheel)
{
	mX = x;
	mY = y;

	mWheel = wheel;

	if (x < 0)	mX = 0;
	if (y < 0)	mY = 0;
	if (x > mW)	mX = mW;
	if (y > mH)	mY = mH;
}


