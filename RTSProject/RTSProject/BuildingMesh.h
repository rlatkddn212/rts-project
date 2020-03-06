/**
 * °Ç¹° ¸ðµ¨
 */
#pragma once

#include "StaticMesh.h"

class BuildingMesh : public StaticMesh
{
public:
	virtual void							LoadShader();
	virtual void							SetUniform();

	void									SetColor(glm::vec4 color) { mColor = color; }

private:
	glm::vec4								mColor;
};

