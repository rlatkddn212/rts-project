#pragma once

#include "Camera.h"
#include "Texture.h"
#include "Ray.h"
#include "RenderObject.h"
#include "RenderManager.h"

class RTSObject : public RenderObject
{
public:
											RTSObject();
	virtual								   ~RTSObject();
	
	void									MakeModel() {}
	virtual	void							Update(float deltaTime) {}
	virtual void							Render(std::shared_ptr<Camera> camera) {}
	virtual void							AddRender(std::shared_ptr<Camera> camera) {}

	virtual bool							Intersect(Ray ray) { return false;  }

	void									SetPosition(glm::vec3 p) { mPos = p; }
	glm::vec3 								GetPosition() { return mPos; }
	void									SetRotation(glm::vec3 r) { mRot = r; }
	void									SetScale(glm::vec3 s) { mSca = s; }
	void									SetFogTexture(std::shared_ptr<Texture> texture) { mFogTexture = texture; }

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
	std::shared_ptr<Texture>				mFogTexture;
	glm::vec3								mPos, mRot, mSca;
};

