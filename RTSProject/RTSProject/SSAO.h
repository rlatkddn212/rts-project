#pragma once
#include "Texture.h"
#include <vector>

class SSAO
{
public:
										SSAO();
									   ~SSAO();

	bool								Initialize(int width, int height);
	void								Terminate();

	unsigned int						GetSSAOFrameBufferID() const { return mSsaoFrameBufferID; }
	unsigned int						GetBlurFrameBufferID() const { return mBlurFrameBufferID; }
	void								SetSSAOActive(int pos);
	void								SetBlurActive(int pos);

private:
	std::shared_ptr<Texture>			mSSAOTextures;
	std::shared_ptr<Texture>			mBlurTextures;
	unsigned int						mSsaoFrameBufferID;
	unsigned int						mBlurFrameBufferID;
};
