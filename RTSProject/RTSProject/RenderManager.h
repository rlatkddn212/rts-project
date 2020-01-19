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

struct DirectLight
{
	glm::vec3 dir;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
};

class RenderManager : public Singleton<RenderManager>
{
public:
	void									Initialize();
	void									AddQueue(std::shared_ptr<RenderObject> obj);
	std::vector<std::shared_ptr<RenderObject>>	GetQueue();
	void									Render();

	void									Draw3DScene(unsigned int type, std::vector<std::shared_ptr<RenderObject>>& renderObj);
	void									DrawFromGBuffer(std::shared_ptr<Camera> camera);
private:
	std::shared_ptr<VertexArray>			mVerts;
	std::shared_ptr<GBuffer>				mGBuffer;
	std::shared_ptr<Shader>					mLightShader;
	std::shared_ptr<Shader>					mPointLightShader;

	std::condition_variable					cv;
	std::mutex								mMutex;
	std::queue<std::shared_ptr<RenderObject>> q;

	int										mSize;
	int										mCount;
};
