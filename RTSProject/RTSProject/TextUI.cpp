#include "Precompiled.h"
#include "TextUI.h"
#include "RenderManager.h"

using namespace std;

TextUI::TextUI(const std::string& str)
{
	mRenderState = UIRendering;
	mStr = str;
	mScale = 1.0f;
	mPosX = 0;
	mPosY = 0;
	mFont = std::make_shared<Font>();
	mFont->Initialize();
	mTexture = nullptr;

	MakeModel();
}

TextUI::~TextUI()
{
}

void TextUI::MakeModel()
{
	float vertices[] =
	{
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
		0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	mTexture = mFont->RenderText(mStr);

	mSpriteVerts = std::make_shared<VertexArray>(vertices, 4, indices, 6);

	std::vector<std::pair<std::string, int> > shaderCodies;
	mSpriteShader = std::make_shared<Shader>();
	shaderCodies.push_back(make_pair(ReadShaderFile("Sprite.vert"), GL_VERTEX_SHADER));
	shaderCodies.push_back(make_pair(ReadShaderFile("Sprite.frag"), GL_FRAGMENT_SHADER));
	mSpriteShader->BuildShader(shaderCodies);
}

void TextUI::Update()
{
}

void TextUI::Render(std::shared_ptr<Camera> camera)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	mSpriteVerts->SetActive();
	glActiveTexture(GL_TEXTURE0);
	mTexture->SetActive();
	mSpriteShader->SetActive();
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.f),
		glm::vec3(static_cast<float>(mTexture->GetWidth()) * mScale,
			static_cast<float>(mTexture->GetHeight()) * mScale,
			1.0f));

	glm::mat4 transMat = glm::translate(glm::mat4(1.f), 
		glm::vec3(mPosX + (-camera->mWidth + static_cast<float>(mTexture->GetWidth())) / 2,
			mPosY + (camera->mHeight - static_cast<float>(mTexture->GetHeight())) / 2,
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

void TextUI::AddRender(std::shared_ptr<Camera> camera)
{
	std::shared_ptr<TextUI> ro = std::make_shared<TextUI>(*this);
	ro->mCamera = camera;
	RenderManager::GetInstance()->AddQueue(ro);
}
