#pragma once

/**
 * 미니맵에 왼쪽 하단이 지도에서 0, 0 좌표를 가진다.
 */

#include "Precompiled.h"
#include "Texture.h"
#include "Camera.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Unit.h"

class MiniMap
{
public:
											MiniMap(int winSizeX, int winSizeY, int miniX, int miniY);
										   ~MiniMap();

	void									Update(float deltaTime, std::vector<std::shared_ptr<Unit>> unit, std::shared_ptr<Camera> camera);
	void									Render(std::shared_ptr<Camera> camera);

	void									SetMapTexture(std::shared_ptr<Texture> texture) { mMapTexture = texture; }
	void									SetFogTexture(std::shared_ptr<Texture> texture) { mFogTexture = texture; }
	
	// UnitTexture 생성
	void									SetUnitTexture() {}
	bool									RayIntersectPlane(glm::vec3 n, glm::vec3 p0, glm::vec3 org, glm::vec3 dir, float* t);

	bool									IsXYInMiniMap(int mouseX, int mouseY);
	glm::vec2								GetTerrainPos(int mouseX, int mouseY);

private:
	int										mWinX;
	int										mWinY;
	int										mMiniMapX;
	int										mMiniMapY;

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

