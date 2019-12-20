#include "Precompiled.h"
#include "SpriteUI.h"

SpriteUI::SpriteUI()
{
	float vertices[] = {
	-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
	0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
	0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
	-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
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



SpriteUI::~SpriteUI()
{
}

void SpriteUI::SetTexture(std::string & name)
{
	mTexture->Load(name);
}

void SpriteUI::Render(Camera * camera)
{
	mSpriteVerts->SetActive();
	mTexture->SetActive();
	mSpriteShader->SetActive();
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.f), glm::vec3(1.0f, 1.0f, 1.0f));

	glm::mat4 transMat = glm::translate(glm::mat4(1.f), glm::vec3(mPosX + (-camera->mWidth + static_cast<float>(mTexture->GetWidth())) / 2,
		mPosY + (camera->mHeight - static_cast<float>(mTexture->GetHeight())) / 2,
		0.0f));

	glm::mat4 m = {
		{ 2.0f / (float)camera->mWidth, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / (float)camera->mHeight, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};

	mSpriteShader->SetMatrixUniform("uViewProj", m);

	glm::mat4 world = transMat * scaleMat;
	mSpriteShader->SetMatrixUniform("uWorldTransform", world);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
