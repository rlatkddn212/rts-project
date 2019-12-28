#pragma once

#include "StaticMesh.h"

class BuildingMesh : public StaticMesh
{
public:
	virtual void							LoadShader();
	virtual void							SetUniform();

	void									SetColor(glm::vec3 color) { mColor = color; }

private:
	glm::vec3								mColor;
};

