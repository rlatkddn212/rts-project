#pragma once

#include "Texture.h"

class ShadowMap
{
public:
											ShadowMap();
										   ~ShadowMap();

	bool									Initialize(int width, int height);
	void									Terminate();
	unsigned int							GetFrameBufferID() const { return mFramebuffer; }
	void									SetActive(int pos);
	
protected:
	GLuint									mFramebuffer;
	std::shared_ptr<Texture>				mShadowTextures;
	GLuint									shadowWidth;
	GLuint									shadowHeight;
};

