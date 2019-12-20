#include "Precompiled.h"
#include "Font.h"

Font::Font()
{
}

Font::~Font()
{
}

void Font::Initialize()
{
	Load("gulim.ttf");
}

void Font::Terminate()
{
}

bool Font::Load(const std::string& fileName)
{
	auto iter = mFonts.find(fileName);
	if (iter != mFonts.end())
	{
		return true;
	}

	std::vector<int> fontSizes = {
		8, 9,
		10, 11, 12, 14, 16, 18,
		20, 22, 24, 26, 28,
		30, 32, 34, 36, 38,
		40, 42, 44, 46, 48,
		52, 56,
		60, 64, 68,
		72
	};

	for (auto& size : fontSizes)
	{
		TTF_Font* font = TTF_OpenFont(fileName.c_str(), size);
		if (font == nullptr)
		{
			SDL_Log("Failed to load font %s in size %d", fileName.c_str(), size);
			return false;
		}
		mFontData.emplace(size, font);
	}

	return true;
}

void Font::Unload()
{
	for (auto& font : mFontData)
	{
		TTF_CloseFont(font.second);
	}
}

std::shared_ptr<Texture> Font::RenderText(const std::string& textKey, const glm::vec4 color, int pointSize)
{
	std::shared_ptr<Texture> texture = nullptr;

	SDL_Color sdlColor;
	sdlColor.r = static_cast<Uint8>(color.x * 255);
	sdlColor.g = static_cast<Uint8>(color.y * 255);
	sdlColor.b = static_cast<Uint8>(color.z * 255);
	sdlColor.a = 100;

	auto iter = mFontData.find(pointSize);

	if (iter != mFontData.end())
	{
		TTF_Font* font = iter->second;
		const std::string& actualText = textKey;
		SDL_Surface* surf = TTF_RenderUTF8_Blended(font, actualText.c_str(), sdlColor);

		if (surf != nullptr)
		{
			texture = std::make_shared<Texture>();
			texture->CreateFromSurface(surf);
			SDL_FreeSurface(surf);
		}
	}

	return texture;
}
