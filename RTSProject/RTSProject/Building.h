#pragma once

#include "BuildingMesh.h"
#include "Terrain.h"
#include "RTSObject.h"

class Building : public RTSObject
{
public:
											Building(){}
											Building(int type);
	virtual								   ~Building();

	void									MakeModel(int type);

	virtual	void							Update(float deltaTime);
	virtual void							Render(std::shared_ptr<Camera> camera);
	virtual void							AddRender(std::shared_ptr<Camera> camera);

	int										GetType() { return mType; }
	bool									isPossibleBuild(std::shared_ptr<Terrain> terrain, int x, int y);
	void									BuildOnTerrain(std::shared_ptr<Terrain> terrain, int x, int y);
	void									SetColor(glm::vec3 color) { mBuildingMesh->SetColor(color); }
	
	void									SetPosition(glm::vec3 p) { RTSObject::SetPosition(p); mBuildingMesh->SetPosition(p); }
	void									SetRotation(glm::vec3 r) { RTSObject::SetRotation(r); mBuildingMesh->SetRotation(r); }
	void									SetScale(glm::vec3 s) { RTSObject::SetScale(s); mBuildingMesh->SetScale(s); }

private:
	glm::vec3								mColor;
	int										mType;


	std::shared_ptr<BuildingMesh>			mBuildingMesh;
};