#pragma once
#include "Texture.h"
#include <vector>

class GBuffer
{
public:
	enum Type
	{
		EDiffuse = 0,
		ENormal,
		EWorldPos,
		NUM_GBUFFER_TEXTURES
	};

										GBuffer();
										~GBuffer();

	bool								Initialize(int width, int height);
	void								Terminate();

	std::shared_ptr<Texture>			GetTexture(Type type);
	unsigned int						GetFrameBufferID() const { return mFrameBufferID; }
	void								SetTexturesActive();

	void								SetDiffuseActive(int pos);
	void								SetNormalActive(int pos);
	void								SetWorldPosActive(int pos);

private:
	std::vector<std::shared_ptr<Texture>>	mTextures;
	unsigned int						mFrameBufferID;
};
