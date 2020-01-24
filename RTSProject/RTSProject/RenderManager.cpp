/**
 * TODO : Main 쓰레드와 랜더 쓰레드 두 쓰레드에서 접근하도록 코드 변경 필요
 */

#include "Precompiled.h"
#include "RenderManager.h"
#include <random>
#include "OrthoCamera.h"

void PrintScreen(GLuint framebuffer, const std::string& str)
{
	// 1024는 임의의 크기로 수정 가능
	unsigned char* bytes = new unsigned char[1024 * 768 * 3];
	GLuint drawFrameBuffer;
	glGenFramebuffers(1, &drawFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, drawFrameBuffer);

	// create a color attachment texture
	std::shared_ptr<Texture> textureColorbuffer2 = std::make_shared<Texture>();
	textureColorbuffer2->CreateTexture(1024, 768, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_BUFFER_BIT, GL_TEXTURE_2D, textureColorbuffer2->GetTextureID(), 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("frame buffer 완전성 위배");
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFrameBuffer);
	glBlitFramebuffer(0, 0, 1024, 768, 0, 0, 1024, 768, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, drawFrameBuffer);
	glReadPixels(0, 0, 1024, 768, GL_RGB, GL_UNSIGNED_BYTE, bytes);

	SOIL_save_image(str.c_str(), SOIL_SAVE_TYPE_BMP, 1024, 768, 3, bytes);

	delete[] bytes;
	glDeleteFramebuffers(1, &drawFrameBuffer);
}

void RenderManager::Initialize()
{
	mSize = 10000;
	mCount = 0;
	
	// frame Buffer 생성
	mShadowMap = std::make_shared<ShadowMap>();
	mShadowMap->Initialize(1024, 768);

	mGBuffer = std::make_shared<GBuffer>();
	mGBuffer->Initialize(1024, 768);

	mSSAO = std::make_shared<SSAO>();
	mSSAO->Initialize(1024, 768);

	float vertices[] =
	{
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f, // top left
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // top right
		0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // bottom right
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f  // bottom left
	};

	unsigned int indices[] =
	{
		0, 2, 1,
		0, 3, 2
	};

	mVerts = std::make_shared<VertexArray>(vertices, 4, indices, 6);

	mLightShader = std::make_shared<Shader>();
	mPointLightShader = std::make_shared<Shader>();

	mSSAOShader = std::make_shared<Shader>();
	mSSAOBlurShader = std::make_shared<Shader>();

	std::vector<std::pair<std::string, int> > shaderCodies;
	shaderCodies.push_back(make_pair(ReadShaderFile("Sprite.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("GBufferGlobalLight.frag"), GL_FRAGMENT_SHADER));
	mLightShader = std::make_shared<Shader>();
	mLightShader->BuildShader(shaderCodies);
	
	shaderCodies.clear();
	shaderCodies.push_back(make_pair(ReadShaderFile("Sprite.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("GBufferPointLight.frag"), GL_FRAGMENT_SHADER));
	mPointLightShader = std::make_shared<Shader>();
	mPointLightShader->BuildShader(shaderCodies);

	shaderCodies.clear();
	shaderCodies.push_back(make_pair(ReadShaderFile("Sprite.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("ssao.frag"), GL_FRAGMENT_SHADER));
	mSSAOShader = std::make_shared<Shader>();
	mSSAOShader->BuildShader(shaderCodies);

	shaderCodies.clear();
	shaderCodies.push_back(make_pair(ReadShaderFile("Sprite.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("ssao_blur.frag"), GL_FRAGMENT_SHADER));
	mSSAOBlurShader = std::make_shared<Shader>();
	mSSAOBlurShader->BuildShader(shaderCodies);
}

void RenderManager::AddQueue(std::shared_ptr<RenderObject> obj)
{
	std::unique_lock<std::mutex> lk(mMutex);
	cv.wait(lk, [&]() { return mCount < mSize; });
	q.push(obj);
	mCount++;
	lk.unlock();
	cv.notify_one();
}

std::vector<std::shared_ptr<RenderObject>> RenderManager::GetQueue()
{
	std::vector<std::shared_ptr<RenderObject>> ret;
	std::unique_lock<std::mutex> lk(mMutex);
	cv.wait(lk, [&]() { return mCount != 0; });
	
	while (!q.empty())
	{
		ret.push_back(q.front());
		q.pop();
		mCount--;
	}

	lk.unlock();
	cv.notify_one();

	return ret;
}

void RenderManager::Render()
{
	std::vector<std::shared_ptr<RenderObject>> renderData = GetQueue();

	DrawGBuffer(mGBuffer->GetFrameBufferID(), renderData);
	DrawShadowMap(mShadowMap->GetFrameBufferID(), renderData);
	DrawSSAO(renderData[0]->mCamera);
	//PrintScreen(mShadowMap->GetFrameBufferID(), "ShadowMap.bmp");
	//PrintScreen(mGBuffer->GetFrameBufferID(), "Helloworld.bmp");
	//PrintScreen(mSSAO->GetSSAOFrameBufferID(), "SSAO.bmp");
	//PrintScreen(mSSAO->GetBlurFrameBufferID(), "SSAOBlur.bmp");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	DrawFromGBuffer(renderData[0]->mCamera);

	for (int i = 0; i < renderData.size(); ++i)
	{
		if (renderData[i]->GetRenderState() == ForwardRendering)
		{
			renderData[i]->UpdateModel();
			renderData[i]->Render(renderData[i]->mCamera);
		}
	}

	for (int i = 0; i < renderData.size(); ++i)
	{
		if (renderData[i]->GetRenderState() == EffectRendering)
		{
			renderData[i]->UpdateModel();
			renderData[i]->Render(renderData[i]->mCamera);
		}
	}

	for (int i = 0; i < renderData.size(); ++i)
	{
		if (renderData[i]->GetRenderState() == UIRendering)
		{
			renderData[i]->UpdateModel();
			renderData[i]->Render(renderData[i]->mCamera);
		}
	}
}

void RenderManager::DrawShadowMap(unsigned int framebuffer, std::vector<std::shared_ptr<RenderObject>>& renderObj)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Clear color buffer/depth buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthMask(GL_TRUE);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	std::shared_ptr<Camera> camera = std::make_shared<OrthoCamera>();
	camera->SetCameraPos(glm::vec3(0.0f, 0.0f, -1.0f));
	camera->SetFocus(glm::vec3(0.0f, 0.0f, 0.0f));

	for (int i = 0; i < renderObj.size(); ++i)
	{
		if (renderObj[i]->GetRenderState() == DeferedRendering)
		{
			renderObj[i]->UpdateModel();
			renderObj[i]->Render(camera);
		}
	}
}

void RenderManager::DrawSSAO(std::shared_ptr<Camera> camera)
{
	glBindFramebuffer(GL_FRAMEBUFFER, mSSAO->GetSSAOFrameBufferID());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDepthMask(GL_TRUE);
	glDisable(GL_DEPTH_TEST);
	mVerts->SetActive();
	mSSAOShader->SetActive();
	mGBuffer->SetTexturesActive();

	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
	std::default_random_engine generator;

	std::vector<glm::vec3> ssaoKernel;
	for (unsigned int i = 0; i < 64; ++i)
	{
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / 64.0;

		scale = glm::lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}

	char buf[128] = {};
	for (unsigned int i = 0; i < 64; ++i)
	{
		sprintf(buf, "samples[%d]", i);
		mSSAOShader->SetVectorUniform(buf, ssaoKernel[i]);
	}

	glm::mat4 scaleMat = glm::scale(glm::mat4(1.f), glm::vec3(1024.0f, 768.0f, 1.0f));

	glm::mat4 transMat = glm::translate(glm::mat4(1.f),
		glm::vec3((-camera->mWidth + static_cast<float>(1024.0f)) / 2, (camera->mHeight - static_cast<float>(768.0f)) / 2, 0.0f));

	glm::mat4 m = {
		{ 2.0f / (float)camera->mWidth, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / (float)camera->mHeight, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};

	glm::mat4 world = transMat * scaleMat;
	mSSAOShader->SetIntUniform("uGDiffuse", 0);
	mSSAOShader->SetIntUniform("uGNormal", 1);
	mSSAOShader->SetIntUniform("uGWorldPos", 2);

	mSSAOShader->SetMatrixUniform("uViewProj", m);
	mSSAOShader->SetMatrixUniform("uWorldTransform", world);
	
	mSSAOShader->SetMatrixUniform("projection", camera->GetProjectionMatrix());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glBindFramebuffer(GL_FRAMEBUFFER, mSSAO->GetBlurFrameBufferID());
	glClear(GL_COLOR_BUFFER_BIT);
	mSSAOBlurShader->SetActive();
	mSSAO->SetSSAOActive(0);
	mSSAOBlurShader->SetMatrixUniform("uViewProj", m);
	mSSAOBlurShader->SetMatrixUniform("uWorldTransform", world);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void RenderManager::DrawGBuffer(unsigned int framebuffer, std::vector<std::shared_ptr<RenderObject>>& renderObj)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Clear color buffer/depth buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	for (int i = 0; i < renderObj.size(); ++i)
	{
		if (renderObj[i]->GetRenderState() == DeferedRendering)
		{
			renderObj[i]->UpdateModel();
			renderObj[i]->Render(renderObj[i]->mCamera);
		}
	}
}

void RenderManager::DrawFromGBuffer(std::shared_ptr<Camera> camera)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDepthMask(GL_TRUE);
	glDisable(GL_DEPTH_TEST);
	mLightShader->SetActive();
	mVerts->SetActive();
	mGBuffer->SetTexturesActive();
	mSSAO->SetBlurActive(3);

	glm::mat4 scaleMat = glm::scale(glm::mat4(1.f), glm::vec3(1024.0f, 768.0f, 1.0f));

	glm::mat4 transMat = glm::translate(glm::mat4(1.f),
		glm::vec3((-camera->mWidth + static_cast<float>(1024.0f)) / 2, (camera->mHeight - static_cast<float>(768.0f)) / 2, 0.0f));

	glm::mat4 m = {
		{ 2.0f / (float)camera->mWidth, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / (float)camera->mHeight, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};

	glm::mat4 world = transMat * scaleMat;
	mLightShader->SetIntUniform("uGDiffuse", 0);
	mLightShader->SetIntUniform("uGNormal", 1);
	mLightShader->SetIntUniform("uGWorldPos", 2);
	mLightShader->SetMatrixUniform("uViewProj", m);
	mLightShader->SetMatrixUniform("uWorldTransform", world);

	mLightShader->SetVectorUniform("CameraPos", camera->GetCameraPos());
	mLightShader->SetVectorUniform("AmbientLight", glm::vec3(0.6f, 0.6f, 0.6f));
	mLightShader->SetVectorUniform("Direction", glm::vec3(0.0f, -0.4f, -0.4f));
	mLightShader->SetVectorUniform("DiffuseColor", glm::vec3(1.0f, 1.0f, 1.0f));
	mLightShader->SetVectorUniform("SpecColor", glm::vec3(1.0f, 1.0f, 1.0f));
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, mGBuffer->GetFrameBufferID());
	int width = static_cast<int>(1024.0);
	int height = static_cast<int>(768.0);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
}
