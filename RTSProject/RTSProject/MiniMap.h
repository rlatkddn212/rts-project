#pragma once

#include "Precompiled.h"
#include "Texture.h"
#include "Camera.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Unit.h"

class MiniMap
{
public:
											MiniMap();
										   ~MiniMap();

	void									Update(float deltaTime, std::vector<std::shared_ptr<Unit>> unit, std::shared_ptr<Camera> camera);
	void									Render(std::shared_ptr<Camera> camera);

	void									SetMapTexture(std::shared_ptr<Texture> texture) { mMapTexture = texture; }
	void									SetFogTexture(std::shared_ptr<Texture> texture) { mFogTexture = texture; }
	
	// UnitTexture »ý¼º
	void									SetUnitTexture() {}
	bool									RayIntersectPlane(glm::vec3 n, glm::vec3 p0, glm::vec3 org, glm::vec3 dir, float* t);

	std::shared_ptr<Texture>				mPositionTexture;
	GLuint									mFrameBuffer;
	GLuint									mVao;
	GLuint									mVertexArray;
	GLuint									mVertexBuffer;

	std::shared_ptr<Shader>					mLineShader;
	std::shared_ptr<Shader>					mShader;
	std::shared_ptr<Texture>				mMapTexture;
	std::shared_ptr<Texture>				mFogTexture;
	std::shared_ptr<Texture>				mUnitTexture;
};

