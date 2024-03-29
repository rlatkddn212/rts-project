#include "Precompiled.h"
#include "Effect.h"
#include "EffectResourcePool.h"
#include "Shader.h"

using namespace std;

Effect::Effect()
{
	mRenderState = EffectRendering;
	mTime = 0.0f;
	mColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Update(float timeDelta)
{
}

void ParticleSystem::Render(std::shared_ptr<Camera> camera)
{
	
}

void ParticleSystem::AddRender(std::shared_ptr<Camera> camera)
{
}

bool ParticleSystem::IsDead()
{
	return false;
}

void ParticleSystem::RenderBatch(int start, int batchSize)
{
}

MagicShow::MagicShow(int noParticles, glm::vec3 _origin)
{
}

void MagicShow::Update(float timeDelta)
{
}

bool MagicShow::IsDead()
{
	return false;
}

Smoke::Smoke(int noParticles, glm::vec3 origin)
{
}

void Smoke::Update(float timeDelta)
{
}

bool Smoke::IsDead()
{
	return false;
}

EffectSpell::EffectSpell(glm::vec3 _pos)
{

}

void EffectSpell::Update(float timeDelta)
{
	mTime += timeDelta;
}

void EffectSpell::Render(std::shared_ptr<Camera> camera)
{
}

void EffectSpell::AddRender(std::shared_ptr<Camera> camera)
{
}

bool EffectSpell::IsDead()
{
	return false;
}

EffectFireBall::EffectFireBall(glm::vec3& bonePos, std::shared_ptr<RTSObject> target, RTSObject* attacker)
{
	mSrcBone = bonePos + glm::vec3(0.0f, 1.0f, 0.0f);

	if (0.0f > mSrcBone.x || 100.0f < mSrcBone.x || 0.0f > mSrcBone.y || 100.0f < mSrcBone.y)
	{
		printf("error\n");
	}
	mTarget = target;
	mAttacker = attacker;
	mColor.w = 0.0f;
	mSpeed = 10.0f;
	mDamage = mAttacker->GetDamege();
	mPrc = 0.0f;
	mTime = 0.0f;

	if (mTarget != nullptr)
	{
		mDest = mTarget->GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else
	{
		printf("Erorr\n");
	}

	mTransMat.Init(mSrcBone, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
}

void EffectFireBall::Update(float timeDelta)
{
	mTransMat.mRot += glm::vec3(0.5f, 0.5f, 0.5f) * timeDelta;

	if (mTime < 1.0f)
	{
		mTime += timeDelta;
		mTransMat.mTrans = mSrcBone;
		mTransMat.mScale = glm::vec3(1.0f, 1.0f, 1.0f) * mTime;
		mColor.w = mTime;

		if (mTime >= 1.0f)
		{
			mPrc = 0.0f;
			mColor.w = mTime * 0.5f;
			mSource = mTransMat.mTrans;
			mLength = glm::distance(mSource, mDest);
		}
	}
	else if (mPrc < 1.0f)
	{
		if (mLength == 0.0)
		{
			printf("Effect Error\n");
			assert(0);
		}
		mPrc += (mSpeed * timeDelta) / mLength;
		mTransMat.mTrans = mSource;
		// mTransMat.mScale += glm::vec3(1.0f, 1.0f, 1.0f) * timeDelta;
		if (mPrc >= 1.0f && mTarget != nullptr)
		{
			mTarget->TakeDamege(mAttacker->GetDamege());
			// ���� ����
		}
	}
	else
	{
		// mPrc += (mSpeed * timeDelta) / mLength;
		mTransMat.mScale += glm::vec3(1.0f, 1.0f, 1.0f) * timeDelta;
		mColor.w -= timeDelta * 0.5f;
	}
}

void EffectFireBall::Render(std::shared_ptr<Camera> camera)
{
	glDisable(GL_DEPTH_TEST);
	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	EffectResourcePool::GetInstance()->SetActiveVertex();
	EffectResourcePool::GetInstance()->SetActiveShader();
	EffectResourcePool::GetInstance()->SetActiveTextureFireball();

	shared_ptr<Shader> shader = EffectResourcePool::GetInstance()->GetShader();

	glm::mat4 mat = camera->GetProjectionMatrix() * camera->GetViewMatrix();

	glm::vec3 rotations[] = { glm::vec3(0.0f, 0.0f, 0.0f),
								glm::vec3(glm::pi<float>() * 0.5f, 0.0f, 0.0f),
								glm::vec3(0.0f, glm::pi<float>() * 0.5f, 0.0f),
								glm::vec3(0.0f, 0.0f, glm::pi<float>() * 0.5f) };

	glm::vec3 positions[] = { GetPosition(mPrc),
								GetPosition(mPrc - (0.3f / mLength)),
								GetPosition(mPrc - (0.6f / mLength)),
								GetPosition(mPrc - (0.8f / mLength)),
								GetPosition(mPrc - (1.f / mLength)) };

	glm::vec3 scales[] = { mTransMat.mScale,
							mTransMat.mScale * 0.8f,
							mTransMat.mScale * 0.6f,
							mTransMat.mScale * 0.4f,
							mTransMat.mScale * 0.2f };

	TransformMat tranMat;
	for (size_t t = 0; t < 5; t++)
	{
		for (size_t i = 0; i < 4; ++i)
		{
			tranMat.mTrans = positions[t];
			tranMat.mRot = mTransMat.mRot + rotations[i];
			tranMat.mScale = scales[t];
			shader->SetMatrixUniform("uWorldTransform", tranMat.calc());
			shader->SetMatrixUniform("uViewProj", mat);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

void EffectFireBall::AddRender(std::shared_ptr<Camera> camera)
{
	std::shared_ptr<EffectFireBall> ro = std::make_shared<EffectFireBall>(*this);
	ro->mCamera = camera;
	RenderManager::GetInstance()->AddQueue(ro);
}

bool EffectFireBall::IsDead()
{
	return mColor.w < 0.0f || mTarget == nullptr;
}

glm::vec3 EffectFireBall::GetPosition(float p)
{
	if (p < 0.0f) 
	{
		p = 0.0f;
	}
	return (1.0f - p) * mTransMat.mTrans + p * mDest;
}

EffectFire::EffectFire(glm::vec3 pos, glm::vec3 scale)
{
}

EffectFire::~EffectFire()
{
}

void EffectFire::Update(float timeDelta)
{
}

void EffectFire::Render(std::shared_ptr<Camera> camera)
{
}

void EffectFire::AddRender(std::shared_ptr<Camera> camera)
{
}

bool EffectFire::IsDead()
{
	return false;
}

void EffectFire::Kill()
{
}
