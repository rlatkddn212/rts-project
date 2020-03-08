/**
 * TODO : Main ������� ���� ������ �� �����忡�� �����ϵ��� �ڵ� ���� �ʿ�
 */

#include "Precompiled.h"
#include "RenderManager.h"
#include <random>
#include "OrthoCamera.h"

void PrintScreen(GLuint framebuffer, const std::string& str)
{
	// 1024�� ������ ũ��� ���� ����
	unsigned char* bytes = new unsigned char[1024 * 768 * 3];
	GLuint drawFrameBuffer;
	glGenFramebuffers(1, &drawFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, drawFrameBuffer);

	// create a color attachment texture
	std::shared_ptr<Texture> textureColorbuffer2 = std::make_shared<Texture>();
	textureColorbuffer2->CreateDepthTexture(1024, 768);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureColorbuffer2->GetTextureID(), 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("frame buffer ������ ����");
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
	mFrame = 1;
	
	// frame Buffer ����
	mShadowMap = std::make_shared<ShadowMap>();
	mShadowMap->Initialize(2048 * 4, 2048 * 4);

	mGBuffer = std::make_shared<GBuffer>();
	mGBuffer->Initialize(1024, 768);

	mSSAO = std::make_shared<SSAO>();
	mSSAO->Initialize(1024, 768);

	mFogOfWar = std::make_shared<FogOfWar>(1024, 768);

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

	std::vector<std::pair<std::string, int> > shaderCodies;
	shaderCodies.push_back(make_pair(ReadShaderFile("Sprite.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("gbuffer_global_light.frag"), GL_FRAGMENT_SHADER));
	mLightShader = std::make_shared<Shader>();
	mLightShader->BuildShader(shaderCodies);
	
	shaderCodies.clear();
	shaderCodies.push_back(make_pair(ReadShaderFile("Sprite.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("gbuffer_point_light.frag"), GL_FRAGMENT_SHADER));
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


	shaderCodies.clear();
	shaderCodies.push_back(make_pair(ReadShaderFile("Sprite.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("Sprite.frag"), GL_FRAGMENT_SHADER));
	mTestShader = std::make_shared<Shader>();
	mTestShader->BuildShader(shaderCodies);
}

/**
 * PreRendering Thead���� �������� ������Ʈ�� Queue�� �߰�
 * @param obj : �������� ������Ʈ
 */
void RenderManager::AddQueue(std::shared_ptr<RenderObject> obj)
{
	std::unique_lock<std::mutex> lk(mMutex);
	// ī���Ͱ� �ʰ��ϸ� ��ٸ���.
	mCV.wait(lk, [&]() { return mCount < mSize; });
	mQueue.push(obj);
	++mCount;

	lk.unlock();
	mCV.notify_one();
}

/**
 * Rendering Thread���� Queue�� ����� ������Ʈ�� ��������.
 * @return : RenderObject�� ����
 */
std::vector<std::shared_ptr<RenderObject>> RenderManager::GetQueue()
{
	std::vector<std::shared_ptr<RenderObject>> ret;
	std::unique_lock<std::mutex> lk(mMutex);
	mCV.wait(lk, [&]() { return mCount != 0; });
	
	while (!mQueue.empty())
	{
		ret.push_back(mQueue.front());
		mQueue.pop();
		mCount--;
	}

	lk.unlock();
	mCV.notify_one();
	
	mFrame++;
	return ret;
}

void RenderManager::Render()
{
	std::vector<std::shared_ptr<RenderObject>> renderData = GetQueue();
	
	std::vector<std::shared_ptr<RTSObject>> rtsObjectVec;
	for (size_t i = 0; i < renderData.size(); ++i)
	{
		std::shared_ptr<RTSObject> rtsobj = std::dynamic_pointer_cast<RTSObject>(renderData[i]);
		if (rtsobj != nullptr)
		{
			rtsObjectVec.push_back(rtsobj);
		}
	}

	mFogOfWar->MakeFogTexture(rtsObjectVec);
	
	std::shared_ptr<Texture> fog = mFogOfWar->GetFogTexture();
	for (size_t i = 0; i < renderData.size(); ++i)
	{
		renderData[i]->SetFogTexture(fog);
	}

	std::shared_ptr<Camera> camera = std::make_shared<OrthoCamera>();
	camera->SetCameraPos(glm::vec3(100.0f, 50.0f, -100.0f));
	camera->SetFocus(glm::vec3(0.0f, 0.0f, 0.0f));

	DrawGBuffer(mGBuffer->GetFrameBufferID(), renderData);
	DrawShadowMap(mShadowMap->GetFrameBufferID(), camera, renderData);
	DrawSSAO(renderData[0]->mCamera);
	//PrintScreen(mShadowMap->GetFrameBufferID(), "ShadowMap.bmp");
	//PrintScreen(mGBuffer->GetFrameBufferID(), "Helloworld.bmp");
	//PrintScreen(mSSAO->GetSSAOFrameBufferID(), "SSAO.bmp");
	//PrintScreen(mSSAO->GetBlurFrameBufferID(), "SSAOBlur.bmp");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	DrawFromGBuffer(renderData[0]->mCamera, camera);
	//DrawDepthBuffer(renderData[0]->mCamera);

	for (size_t i = 0; i < renderData.size(); ++i)
	{
		if (renderData[i]->GetRenderState() == ForwardRendering)
		{
			renderData[i]->UpdateModel();
			renderData[i]->Render(renderData[i]->mCamera);
		}
	}

	for (size_t i = 0; i < renderData.size(); ++i)
	{
		if (renderData[i]->GetRenderState() == EffectRendering)
		{
			renderData[i]->UpdateModel();
			renderData[i]->Render(renderData[i]->mCamera);
		}
	}

	for (size_t i = 0; i < renderData.size(); ++i)
	{
		if (renderData[i]->GetRenderState() == UIRendering)
		{
			renderData[i]->UpdateModel();
			renderData[i]->Render(renderData[i]->mCamera);
		}
	}
}

void RenderManager::DrawShadowMap(unsigned int framebuffer, std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<RenderObject>>& renderObj)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, 2048 * 4, 2048 * 4);
	// Clear color buffer/depth buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthMask(GL_TRUE);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	for (size_t i = 0; i < renderObj.size(); ++i)
	{
		if (renderObj[i]->GetRenderState() == DeferedRendering)
		{
			renderObj[i]->UpdateModel();
			renderObj[i]->RenderShadow(camera);
		}
	}
	glViewport(0, 0, 1024, 768);
}

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
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

	mGBuffer->SetWorldPosActive(0);
	mGBuffer->SetNormalActive(1);

	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
	std::default_random_engine generator;

	// ���� Ŀ�� ����
	std::vector<glm::vec3> ssaoKernel;
	for (size_t i = 0; i < 64; ++i)
	{
		// [-1~1], [-1~1], [0~1] ���� ����
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		// ���̸� 1�� ����� �ݱ��� �����.
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);

		// ������ �� �߽ɿ� ���� �����ǵ��� ��ġ�� �����Ѵ�.
		float scale = float(i) / 64.0;
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}

	// ���� uniform �߰�
	char buf[128] = {};
	for (size_t i = 0; i < 64; ++i)
	{
		sprintf_s(buf, "samples[%d]", i);
		mSSAOShader->SetVectorUniform(buf, ssaoKernel[i]);
	}

	// ȸ���� �ʿ��� 4x4 ������ �ؽ��� ����
	std::vector<glm::vec3> ssaoNoise;
	for (size_t i = 0; i < 16; i++)
	{
		// z �� ȸ���̶� z ���� 0
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f);
		ssaoNoise.push_back(noise);
	}

	unsigned int noiseTexture; 
	glActiveTexture(GL_TEXTURE2);
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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

	mSSAOShader->SetMatrixUniform("uViewProj", m);
	mSSAOShader->SetMatrixUniform("uWorldTransform", world);
	
	mSSAOShader->SetMatrixUniform("uProjectionMat", camera->GetProjectionMatrix());
	mSSAOShader->SetMatrixUniform("uViewMat", camera->GetViewMatrix());

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

	for (size_t i = 0; i < renderObj.size(); ++i)
	{
		if (renderObj[i]->GetRenderState() == DeferedRendering)
		{
			renderObj[i]->UpdateModel();
			renderObj[i]->Render(renderObj[i]->mCamera);
		}
	}
}

void RenderManager::DrawFromGBuffer(std::shared_ptr<Camera> camera, std::shared_ptr<Camera> lightCamera)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDepthMask(GL_TRUE);
	glDisable(GL_DEPTH_TEST);
	mLightShader->SetActive();
	mVerts->SetActive();
	mGBuffer->SetTexturesActive();
	mSSAO->SetBlurActive(3);
	mShadowMap->SetActive(4);

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

	// vp mat
	mLightShader->SetMatrixUniform("cameraMatrix", camera->GetViewMatrix());
	mLightShader->SetMatrixUniform("projMatrix", camera->GetProjectionMatrix());
	// ���� ���� view
	mLightShader->SetMatrixUniform("worldToLightViewMatrix", lightCamera->GetViewMatrix());
	mLightShader->SetMatrixUniform("lightViewToProjectionMatrix", lightCamera->GetProjectionMatrix());

	mLightShader->SetVectorUniform("CameraPos", camera->GetCameraPos());
	mLightShader->SetVectorUniform("AmbientLight", glm::vec3(0.6f, 0.6f, 0.6f));
	mLightShader->SetVectorUniform("Direction", glm::vec3(0.5f, -0.5f, -0.5f));
	mLightShader->SetVectorUniform("DiffuseColor", glm::vec3(1.0f, 1.0f, 1.0f));
	mLightShader->SetVectorUniform("SpecColor", glm::vec3(1.0f, 1.0f, 1.0f));
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
}

void RenderManager::DrawDepthBuffer(std::shared_ptr<Camera> camera)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDepthMask(GL_TRUE);
	glDisable(GL_DEPTH_TEST);
	mTestShader->SetActive();
	mVerts->SetActive();
	mShadowMap->SetActive(0);

	glm::mat4 scaleMat = glm::scale(glm::mat4(1.f), glm::vec3(1024.0f, 768.0f, 1.0f));
	glm::mat4 transMat = glm::translate(glm::mat4(1.f),
						glm::vec3((-camera->mWidth + static_cast<float>(1024.0f)) / 2,
						(camera->mHeight - static_cast<float>(768.0f)) / 2, 0.0f));

	glm::mat4 m = {
		{ 2.0f / (float)camera->mWidth, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / (float)camera->mHeight, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};

	glm::mat4 world = transMat * scaleMat;
	mTestShader->SetMatrixUniform("uViewProj", m);
	mTestShader->SetMatrixUniform("uWorldTransform", world);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
}
