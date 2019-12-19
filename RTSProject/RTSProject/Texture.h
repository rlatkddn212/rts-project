#pragma once
#include "Precompiled.h"
#include <string>

class Texture
{
public:
											Texture();
										   ~Texture();

	bool									Load(const std::string& fileName);
	bool									LoadRawData(unsigned char* image, int w, int h, int channels = 4);

	void									Unload();
	void									CreateFromSurface(struct SDL_Surface* surface);

	void									SetActive();

	int										GetWidth() const { return mWidth; }
	int										GetHeight() const { return mHeight; }
private:
	unsigned int							mTextureID;
	int										mWidth;
	int										mHeight;
};
