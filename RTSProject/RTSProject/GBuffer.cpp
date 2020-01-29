#include "Precompiled.h"
#include "GBuffer.h"

using namespace std;

GBuffer::GBuffer()
 :mFrameBufferID(0)
{

}

GBuffer::~GBuffer()
{

}

bool GBuffer::Initialize(int width, int height)
{
	glGenFramebuffers(1, &mFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	// 출력 포트를 여러 사용한다.
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; ++i)
	{
		shared_ptr<Texture> tex = std::make_shared<Texture>();
		tex->CreateRenderTexture(width, height, GL_RGB32F);
		mTextures.emplace_back(tex);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, tex->GetTextureID(), 0);
	}

	std::vector<GLenum> attachments;
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; ++i)
	{
		attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
	}

	glDrawBuffers(static_cast<GLsizei>(attachments.size()),	attachments.data());

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Terminate();
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void GBuffer::Terminate()
{
	glDeleteFramebuffers(1, &mFrameBufferID);
}

std::shared_ptr<Texture> GBuffer::GetTexture(Type type)
{
	return mTextures[type];
}

void GBuffer::SetTexturesActive()
{
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		mTextures[i]->SetActive();
	}
}

void GBuffer::SetDiffuseActive(int pos)
{
	glActiveTexture(GL_TEXTURE0 + pos);
	mTextures[0]->SetActive();
}

void GBuffer::SetNormalActive(int pos)
{
	glActiveTexture(GL_TEXTURE0 + pos);
	mTextures[1]->SetActive();
}

void GBuffer::SetWorldPosActive(int pos)
{
	glActiveTexture(GL_TEXTURE0 + pos);
	mTextures[2]->SetActive();
}
