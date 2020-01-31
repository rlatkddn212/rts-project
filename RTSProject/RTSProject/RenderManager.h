#pragma once
#include "Singleton.h"
#include "RenderObject.h"
#include "Camera.h"
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include "VertexArray.h"
#include "GBuffer.h"
#include "Shader.h"
#include "SSAO.h"
#include "ShadowMap.h"

class RenderManager : public Singleton<RenderManager>
{
public:
	void									Initialize();
	void									AddQueue(std::shared_ptr<RenderObject> obj);
	std::vector<std::shared_ptr<RenderObject>>	GetQueue();
	void									Render();

	void									DrawShadowMap(unsigned int framebuffer, std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<RenderObject>>& renderObj);
	void									DrawSSAO(std::shared_ptr<Camera> camera);
	void									DrawGBuffer(unsigned int type, std::vector<std::shared_ptr<RenderObject>>& renderObj);
	void									DrawFromGBuffer(std::shared_ptr<Camera> camera, std::shared_ptr<Camera> lightCamera);
	void									DrawDepthBuffer(std::shared_ptr<Camera> camera);

private:
	std::shared_ptr<VertexArray>			mVerts;
	
	std::shared_ptr<ShadowMap>				mShadowMap;
	std::shared_ptr<GBuffer>				mGBuffer;
	std::shared_ptr<SSAO>					mSSAO;

	std::shared_ptr<Shader>					mLightShader;
	std::shared_ptr<Shader>					mPointLightShader;

	std::shared_ptr<Shader>					mSSAOShader;
	std::shared_ptr<Shader>					mSSAOBlurShader;
	std::shared_ptr<Shader>					mTestShader;

	std::condition_variable					mCV;
	std::mutex								mMutex;
	std::queue<std::shared_ptr<RenderObject>> mQueue;

	int										mSize;
	int										mCount;
};
