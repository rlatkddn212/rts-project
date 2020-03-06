/**
 * 미니맵에 왼쪽 하단이 지도에서 0, 0 좌표를 가진다.
 */

#pragma once

#include "Texture.h"
#include "Camera.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Unit.h"
#include "RenderObject.h"
#include "RenderManager.h"

class MiniMap : public RenderObject
{
public:
											MiniMap() {}
											MiniMap(int winSizeX, int winSizeY, int miniX, int miniY);
										   ~MiniMap();

	void									MakeModel();
	void									UpdateModel();
	void									Update(float deltaTime, const std::vector<std::shared_ptr<Unit>>& unit, std::shared_ptr<Camera> camera);
	void									Render(std::shared_ptr<Camera> camera);
	virtual void							AddRender(std::shared_ptr<Camera> camera);

	void									SetMapTexture(std::shared_ptr<Texture> texture) { mMapTexture = texture; }
	
	// UnitTexture 생성
	void									SetUnitTexture() {}
	bool									RayIntersectPlane(glm::vec3 n, glm::vec3 p0, glm::vec3 org, glm::vec3 dir, float* t);

	bool									IsXYInMiniMap(int mouseX, int mouseY);
	glm::vec2								GetTerrainPos(int mouseX, int mouseY);

	GLuint									GetVao() { return mVao; }
	GLuint									GetVertexArray() { return mVertexArray; }
	GLuint									GetVertexBuffer() { return mVertexBuffer; }
	GLuint									GetFrameBuffer() { return mFrameBuffer; }

	std::shared_ptr<Shader>					GetLineShader() { return mLineShader; }
	std::shared_ptr<Shader>					GetShader() { return mShader; }

	std::shared_ptr<Texture>				GetPositionTexture() { return mPositionTexture; }
	std::shared_ptr<Texture>				GetMapTexture() { return mMapTexture; }
	std::shared_ptr<Texture>				GetFogTexture() { return mFogTexture; }
	std::shared_ptr<Texture>				GetUnitTexture() { return mUnitTexture; }

private:
	std::vector<glm::vec3>					mPos;
	std::vector<std::shared_ptr<Unit>>		mUnit;
	int										mWinX;
	int										mWinY;
	int										mMiniMapX;
	int										mMiniMapY;

	GLuint									mFrameBuffer;
	GLuint									mVao;
	GLuint									mVertexArray;
	GLuint									mVertexBuffer;

	std::shared_ptr<Shader>					mLineShader;
	std::shared_ptr<Shader>					mShader;

	std::shared_ptr<Texture>				mPositionTexture;
	std::shared_ptr<Texture>				mMapTexture;
	std::shared_ptr<Texture>				mUnitTexture;
};

