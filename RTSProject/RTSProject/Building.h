#pragma once

#include "StaticMesh.h"
#include "Terrain.h"

class Building : public StaticMesh
{
public:
											Building(int type);
										   ~Building();
	virtual void							LoadShader();

	int										GetType() { return mType; }
	bool									isPossibleBuild(std::shared_ptr<Terrain> terrain, int x, int y);
	void									BuildOnTerrain(std::shared_ptr<Terrain> terrain, int x, int y);
	void									SetColor(glm::vec3 color) { mColor = color; }

	virtual void							SetUniform();

private:
	glm::vec3								mColor;
	int										mType;
};