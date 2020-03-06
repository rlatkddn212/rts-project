#pragma once
#include "Camera.h"
#include "Texture.h"

enum RenderState
{
	DeferedRendering,
	ForwardRendering,
	EffectRendering,
	AlphaRendering,
	UIRendering,
};

class RenderObject
{
public:
	virtual void							RenderShadow(std::shared_ptr<Camera> camera);
	virtual void							Render(std::shared_ptr<Camera> camera);
	virtual void							AddRender(std::shared_ptr<Camera> camera);
	virtual void							MakeModel();
	virtual void							UpdateModel();
	virtual void							SetFogTexture(std::shared_ptr<Texture> texture) { mFogTexture = texture; }

	RenderState								GetRenderState() { return mRenderState; }

	std::shared_ptr<Texture>				mFogTexture;
	std::shared_ptr<Camera>					mCamera;
	RenderState								mRenderState;
};