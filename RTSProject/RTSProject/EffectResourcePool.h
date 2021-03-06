/**
 * EffectResourcePool
 * 싱글턴 클래스로 Game을 시작할 때 이펙트 파일들을 읽고 저장해둔다.
 */

#pragma once
#include "Singleton.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Shader.h"

class EffectResourcePool : public Singleton<EffectResourcePool>
{
public:
	void								Initialize();
	void								Load();
	void								SetActiveVertex() { mVerts->SetActive(); }
	void								SetActiveShader() { mShader->SetActive(); }

	std::shared_ptr<VertexArray>		GetVertex() { return mVerts; }
	std::shared_ptr<Shader>				GetShader() { return mShader; }
	std::shared_ptr<Texture>			GetTexture() { }

	void								SetActiveTextureRunes() { mRunesTexture->SetActive(); }
	void								SetActiveTextureCloud() { mCloudTexture->SetActive(); }
	void								SetActiveTextureFireball() { mFireballTexture->SetActive(); }
	void								SetActiveTextureNoise() { mNoiseTexture->SetActive(); }
	void								SetActiveTextureFire() { mFireTexture->SetActive(); }

private:
	std::shared_ptr<VertexArray>		mVerts;
	std::shared_ptr<Shader>				mShader;

	std::shared_ptr<Texture>			mRunesTexture;
	std::shared_ptr<Texture>			mCloudTexture;
	std::shared_ptr<Texture>			mFireballTexture;
	std::shared_ptr<Texture>			mNoiseTexture;
	std::shared_ptr<Texture>			mFireTexture;
};

