#pragma once
class FogOfWar
{
public:
											FogOfWar();

	virtual	void							Update(float deltaTime);
	virtual void							Render();


	GLuint									mVao;
	GLuint									mSightTexture;
	GLuint									mVisiableTexture;
	GLuint									mVisitedTexture;
	GLuint									mFogTexture;

};

