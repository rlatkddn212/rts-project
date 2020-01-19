#pragma once
#include "Camera.h"
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
	virtual void							Render(std::shared_ptr<Camera> camera);
	virtual void							AddRender(std::shared_ptr<Camera> camera);
	virtual void							MakeModel();
	virtual void							UpdateModel();

	RenderState								GetRenderState() { return mRenderState; }

	std::shared_ptr<Camera>					mCamera;
	RenderState								mRenderState;
};