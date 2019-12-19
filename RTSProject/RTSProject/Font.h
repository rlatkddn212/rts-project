#pragma once
#include "Precompiled.h"
#include <string>
#include <unordered_map>
#include "Texture.h"
#include <SDL/SDL_ttf.h>

class Font
{
public:
	Font();
	~Font();

	void Initialize();
	void Terminate();

	bool Load(const std::string& fileName);
	void Unload();

	std::shared_ptr<Texture> RenderText(const std::string& textKey,
		const glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		int pointSize = 30);
private:

	std::unordered_map<std::string, class Font*> mFonts;
	std::unordered_map<std::string, std::string> mText;
	std::unordered_map<int, TTF_Font*> mFontData;
};
