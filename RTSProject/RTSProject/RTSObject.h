#pragma once
#include "CameraRay.h"
#include "Camera.h"
#include "Texture.h"
#include "RenderObject.h"
#include "RenderManager.h"
class CameraRay;
class RTSObject : public RenderObject
{
public:
											RTSObject();
	virtual								   ~RTSObject();
	
	void									MakeModel() {}
	virtual	void							Update(float deltaTime) {}
	virtual void							Render(std::shared_ptr<Camera> camera) {}
	virtual void							AddRender(std::shared_ptr<Camera> camera) {}

	virtual bool							Intersect(CameraRay ray);

	void									SetPosition(glm::vec3 p) { mPos = p; }
	glm::vec3 								GetPosition() { return mPos; }
	void									SetRotation(glm::vec3 r) { mRot = r; }
	void									SetScale(glm::vec3 s) { mSca = s; }

	double									GetRange() { return mRange; }
	double									GetSight() { return mSight; }
	double									GetDamege() { return mDamege; }
	void									SetAttackTime(double time) { mAttackTime = time; }
	double									GetAttackTime() { return mAttackTime; }

	double									GetAttackSpeed() { return mAttackSpeed; }

	void									SetHealth(int health) { mHealth = health; }
	int										GetHealth() { return mHealth; }
	virtual void							TakeDamege(double damege) { mHealth -= damege; }
	bool									IsDead() { return mHealth <= 0; }

protected:
	int										mHealth;
	int										mMaxHealth;
	double									mAttackSpeed;
	double									mRange;
	double									mDefense;
	double									mSpeed;
	double									mDamege;
	double									mSight;

	double									mAttackTime;

	bool									mIsFog;
	glm::vec3								mPos, mRot, mSca;
};

