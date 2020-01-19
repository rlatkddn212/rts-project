#pragma once
#include "Texture.h"
#include "RTSObject.h"
#include "EffectResourcePool.h"
#include "Unit.h"
#include "RenderObject.h"
#include "RenderManager.h"

struct Particle
{
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	glm::vec4 color;
	float liveTime;
	bool isDead;
};

struct TransformMat
{
	void Init(glm::vec3 trans, glm::vec3 rots, glm::vec3 scale) { mTrans = trans; mRot = rots; mScale = scale; }
	
	glm::mat4 calc() 
	{
		return glm::translate(glm::mat4(1.0f), mTrans) 
			* glm::yawPitchRoll(mRot.x, mRot.y, mRot.z)
			* glm::scale(glm::mat4(1.0f), mScale);
	}
	
	glm::vec3 mTrans;
	glm::vec3 mRot;
	glm::vec3 mScale;
};

class Effect : public RenderObject
{
public:
											Effect();
										   ~Effect() {}
	virtual void							Update(float timeDelta) = 0;
	virtual void							Render(std::shared_ptr<Camera> camera) = 0;
	void									AddRender(std::shared_ptr<Camera> camera) {}

	virtual bool							IsDead() = 0;

protected:
	TransformMat							mTransMat;
	float									mTime;
	glm::vec4								mColor;
};

class ParticleSystem : public Effect
{
public:
											ParticleSystem();
										   ~ParticleSystem();
	void									Update(float timeDelta);
	void									Render(std::shared_ptr<Camera> camera);
	void									AddRender(std::shared_ptr<Camera> camera);

	bool									IsDead();

	void									RenderBatch(int start, int batchSize);
	
	std::vector<std::shared_ptr<Particle>>	mParticles;
	std::shared_ptr<Texture>				mTexture;
	float									mSize;
};

class MagicShow : public ParticleSystem
{
public:
											MagicShow(int noParticles, glm::vec3 _origin);
	void									Update(float timeDelta);
	bool									IsDead();
private:
	glm::vec3								mOrigin;
};

class Smoke : public ParticleSystem
{
public:
											Smoke(int noParticles, glm::vec3 origin);
	void									Update(float timeDelta);
	bool									IsDead();
	void									Kill() { mIsDead = true; }

private:
	glm::vec3								mOrigin;
	bool									mIsDead;
	int										mNumAlive;
};

class EffectSpell : public Effect
{
public:
											EffectSpell(glm::vec3 _pos);
	void									Update(float timeDelta);
	void									Render(std::shared_ptr<Camera> camera);
	void									AddRender(std::shared_ptr<Camera> camera);

	bool									IsDead();

private:
	TransformMat							mMat[10];
};

class EffectFireBall : public Effect
{
public:
											EffectFireBall(glm::vec3& bonePos, std::shared_ptr<RTSObject> target, RTSObject* attacker);
	void									Update(float timeDelta);
	void									Render(std::shared_ptr<Camera> camera);
	void									AddRender(std::shared_ptr<Camera> camera);

	bool									IsDead();
	glm::vec3								GetPosition(float p);

private:
	glm::vec3								mSrcBone;
	float									mSpeed;
	float									mLength;
	float									mPrc;
	int										mDamage;

	glm::vec3								mSource;
	glm::vec3								mDest;
	std::shared_ptr<RTSObject>				mTarget;
	RTSObject*								mAttacker;
};

class EffectFire : public Effect
{
public:
											EffectFire(glm::vec3 pos, glm::vec3 scale);
										   ~EffectFire();
	void									Update(float timeDelta);
	void									Render(std::shared_ptr<Camera> camera);
	void									AddRender(std::shared_ptr<Camera> camera);

	bool									IsDead();
	void									Kill();

private:
	glm::vec3								mTargetScale;
	glm::vec3								mCamEye;
	std::shared_ptr<Smoke>					mSmoke;
	bool									mIsDead;
};