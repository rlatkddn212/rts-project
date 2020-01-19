#pragma once
#include "Precompiled.h"
#include <string>

class Texture
{
public:
											Texture();
										   ~Texture();

	bool									Load(const std::string& fileName);
	void									Unload();

	bool									CreateRenderTexture(int w, int h, int format);
	bool									CreateFromRawData(unsigned char* image, int w, int h, int channels = 4);
	void									CreateFromSurface(struct SDL_Surface* surface);
	void									CreateTexture(int w, int h, unsigned char byte);

	void									SetActive();

	int										GetWidth() const { return mWidth; }
	int										GetHeight() const { return mHeight; }

	GLuint									GetTextureID() { return mTextureID; }

private:
	GLuint									mTextureID;
	int										mWidth;
	int										mHeight;
};
