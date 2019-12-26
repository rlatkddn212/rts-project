#include "Precompiled.h"
#include "Math3D.h"

glm::ivec2 RoundPosition(glm::vec2 pos)
{
	return glm::ivec2(round(pos.x), round(pos.y));
}