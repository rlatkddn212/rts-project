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

	void									Update(float deltaTime, std::vector<std::shared_ptr<Unit>> unit);
	void									Render(std::shared_ptr<Camera> camera);

	void									SetMapTexture(std::shared_ptr<Texture> texture) { mMapTexture = texture; }
	void									SetFogTexture(std::shared_ptr<Texture> texture) { mFogTexture = texture; }
	
	// UnitTexture »ý¼º
	void									SetUnitTexture() {}

	std::shared_ptr<Texture>				mPositionTexture;
	GLuint									mFrameBuffer;
	GLuint									mVao;
	std::shared_ptr<Shader>					mShader;
	std::shared_ptr<Texture>				mMapTexture;
	std::shared_ptr<Texture>				mFogTexture;
	std::shared_ptr<Texture>				mUnitTexture;
};

