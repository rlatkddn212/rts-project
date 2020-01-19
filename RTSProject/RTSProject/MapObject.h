#pragma once
#include "RTSObject.h"
#include "StaticMesh.h"
#include "RenderManager.h"

class MapObject : public RTSObject
{
public:
											MapObject() {}
											MapObject(int type);
	virtual								   ~MapObject();
	
	void									MakeModel(int type);
	virtual	void							Update(float deltaTime);
	virtual void							Render(std::shared_ptr<Camera> camera);
	void									AddRender(std::shared_ptr<Camera> camera);

	void									SetPosition(glm::vec3 p) { RTSObject::SetPosition(p); mStaticMesh->SetPosition(p); }
	void									SetRotation(glm::vec3 r) { RTSObject::SetRotation(r); mStaticMesh->SetRotation(r); }
	void									SetScale(glm::vec3 s) { RTSObject::SetScale(s); mStaticMesh->SetScale(s); }

	std::shared_ptr<StaticMesh>				mStaticMesh;
};