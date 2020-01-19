#include "Precompiled.h"
#include "SpriteUI.h"

SpriteUI::SpriteUI()
{
	mPosX = 0.0f;
	mPosY = 0.0f;
	mScaleX = 0.0f;
	mScaleY = 0.0f;
	
	MakeModel();
}

SpriteUI::~SpriteUI()
{
}

void SpriteUI::MakeModel()
{
	float vertices[] = {
	-1.0f, 1.0f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
	1.0f, 1.0f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
	1.0f,-1.0f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
	-1.0f,-1.0f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = std::make_shared<VertexArray>(vertices, 4, indices, 6);

	std::vector<std::pair<std::string, int> > shaderCodies;

	mSpriteShader = std::make_shared<Shader>();
	shaderCodies.push_back(make_pair(ReadShaderFile("Sprite.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("Sprite.frag"), GL_FRAGMENT_SHADER));
	mSpriteShader->BuildShader(shaderCodies);

	mTexture = std::make_shared<Texture>();
}

void SpriteUI::SetTexture(const std::string & name)
{
	mTexture->Load(name);
}

void SpriteUI::Render(std::shared_ptr<Camera> camera)
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	mSpriteVerts->SetActive();
	glActiveTexture(GL_TEXTURE0);
	mTexture->SetActive();
	mSpriteShader->SetActive();
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.f), glm::vec3(static_cast<float>(mTexture->GetWidth()) * mScaleX,
		static_cast<float>(mTexture->GetHeight()) * mScaleY, 1.0f));

	glm::mat4 transMat = glm::translate(glm::mat4(1.f), 
		glm::vec3(mPosX + (-camera->mWidth + static_cast<float>(mTexture->GetWidth()) * mScaleX) / 2,
		-mPosY + (camera->mHeight - static_cast<float>(mTexture->GetHeight()) * mScaleY) / 2,
		0.0f));

	glm::mat4 m = {
		{ 2.0f / (float)camera->mWidth, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / (float)camera->mHeight, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};

	glm::mat4 world = transMat * scaleMat;
	mSpriteShader->SetMatrixUniform("uViewProj", m);
	mSpriteShader->SetMatrixUniform("uWorldTransform", world);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

void SpriteUI::AddRender(std::shared_ptr<Camera> camera)
{
	std::shared_ptr<SpriteUI> ro = std::make_shared<SpriteUI>(*this);
	ro->mCamera = camera;
	RenderManager::GetInstance()->AddQueue(ro);
}

void SpriteUI::SetUV(float x1, float y1, float x2, float y2)
{
	float vertices[] = 
	{
		-1.0f, 1.0f, 0.f, 0.f, 0.f, 0.0f, x1, y1, // top left
		1.0f, 1.0f, 0.f, 0.f, 0.f, 0.0f, x2, y1, // top right
		1.0f, -1.0f, 0.f, 0.f, 0.f, 0.0f, x2, y2, // bottom right
		-1.0f, -1.0f, 0.f, 0.f, 0.f, 0.0f, x1, y2  // bottom left
	};

	mSpriteVerts->SetVertexBuffer(vertices, 4);
}
