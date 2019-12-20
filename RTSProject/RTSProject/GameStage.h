#pragma once
#include "WindowGroup.h"
#include "HeightMap.h"
#include "Terrain.h"
#include "StaticMesh.h"
#include "Camera.h"
#include "Mouse.h"
#include "SkinnedMesh.h"
#include "AxisObject.h"

class GameStage : public WindowGroup
{
public:
											GameStage();
	virtual								   ~GameStage();
	
	virtual void							Initialize(GLFWwindow* window, int w, int h);
	virtual void							Terminate();
	virtual void							Update(float deltaTime);
	virtual void							Render();

	virtual void							PressKey(bool* keys);
	virtual void							CursorPos(double xPos, double yPos);
	virtual void							MouseButton(int button, int action);
	virtual void							MouseWheel(double yPos);

	std::shared_ptr<HeightMap>				heightMap;
	std::shared_ptr<Terrain>				terrain;
	
	std::vector<std::shared_ptr<SkinnedMesh> >	meshes;
	std::shared_ptr<AxisObject>				axis;

	bool									isLeftPress;
};

