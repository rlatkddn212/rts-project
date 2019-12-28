#pragma once
#include "RTSObject.h"
#include "StaticMesh.h"

class MapObject : public RTSObject
{
public:
											MapObject(int type);
	virtual								   ~MapObject();

	virtual	void							Update(float deltaTime);
	virtual void							Render(std::shared_ptr<Camera> camera);

	void									SetPosition(glm::vec3 p) { RTSObject::SetPosition(p); mStaticMesh->SetPosition(p); }
	void									SetRotation(glm::vec3 r) { RTSObject::SetRotation(r); mStaticMesh->SetRotation(r); }
	void									SetScale(glm::vec3 s) { RTSObject::SetScale(s); mStaticMesh->SetScale(s); }

	std::shared_ptr<StaticMesh>				mStaticMesh;
};

