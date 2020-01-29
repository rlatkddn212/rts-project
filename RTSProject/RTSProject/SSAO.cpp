#include "Precompiled.h"
#include "SSAO.h"

using namespace std;

SSAO::SSAO()
 :mBlurFrameBufferID(0), mSsaoFrameBufferID(0)
{

}

SSAO::~SSAO()
{

}

bool SSAO::Initialize(int width, int height)
{
	glGenFramebuffers(1, &mSsaoFrameBufferID);
	glGenFramebuffers(1, &mBlurFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mSsaoFrameBufferID);

	mSSAOTextures = std::make_shared<Texture>();
	mSSAOTextures->CreateRenderTexture(width, height, GL_RED);
	
	// SSAO 색상 버퍼
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mSSAOTextures->GetTextureID(), 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Terminate();
		return false;
	}

	// 블러
	glBindFramebuffer(GL_FRAMEBUFFER, mBlurFrameBufferID);
	mBlurTextures = std::make_shared<Texture>();
	mBlurTextures->CreateRenderTexture(width, height, GL_RED);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mBlurTextures->GetTextureID(), 0);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Terminate();
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void SSAO::Terminate()
{
	glDeleteFramebuffers(1, &mSsaoFrameBufferID);
	glDeleteFramebuffers(1, &mBlurFrameBufferID);
}

void SSAO::SetSSAOActive(int pos)
{
	glActiveTexture(GL_TEXTURE0 + pos);
	mSSAOTextures->SetActive();
}

void SSAO::SetBlurActive(int pos)
{
	glActiveTexture(GL_TEXTURE0 + pos);
	mBlurTextures->SetActive();
}
