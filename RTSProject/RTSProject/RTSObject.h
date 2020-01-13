#pragma once

#include "Camera.h"
#include "Texture.h"
#include "Ray.h"

class RTSObject
{
public:
											RTSObject();
	virtual								   ~RTSObject();

	virtual	void							Update(float deltaTime) = 0;
	virtual void							Render(std::shared_ptr<Camera> camera) = 0;

	void									SetPosition(glm::vec3 p) { mPos = p; }
	glm::vec3 								GetPosition() { return mPos; }
	void									SetRotation(glm::vec3 r) { mRot = r; }
	void									SetScale(glm::vec3 s) { mSca = s; }
	virtual bool							Intersect(Ray ray) { return false;  }
	void									SetFogTexture(std::shared_ptr<Texture> texture) { mFogTexture = texture; }


	double									GetRange() { return mRange; }
	double									GetSight() { return mSight; }
	double									GetDamege() { return mDamege; }
	void									SetAttackTime(double time) { mAttackTime = time; }
	double									GetAttackTime() { return mAttackTime; }

	double									GetAttackSpeed() { return mAttackSpeed; }

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

	std::shared_ptr<Texture>				mFogTexture;
	glm::vec3								mPos, mRot, mSca;
};

