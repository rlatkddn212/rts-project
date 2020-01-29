#include "Precompiled.h"
#include "ShadowMap.h"

ShadowMap::ShadowMap()
{
}


ShadowMap::~ShadowMap()
{
}

bool ShadowMap::Initialize(int width, int height)
{
	shadowWidth = width;
	shadowHeight = height;

	glGenFramebuffers(1, &mFramebuffer); 
	mShadowTextures = std::make_shared<Texture>();
	mShadowTextures->CreateDepthTexture(width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowTextures->GetTextureID(), 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		Terminate();
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void ShadowMap::Terminate()
{
	glDeleteFramebuffers(1, &mFramebuffer);
}

void ShadowMap::SetActive(int pos)
{
	glActiveTexture(GL_TEXTURE0 + pos);
	mShadowTextures->SetActive();
}
