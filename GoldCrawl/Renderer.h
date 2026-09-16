#pragma once

#include "Prelude.h"
#include <SDL_ttf.h>
#include "Sprite.h"

class Renderer
{
public:
	Renderer(const std::string& title, const int width, const int height, const int scale);
	~Renderer();

	void clear();
	void render() const;

	void drawBox(SDL_Rect bounds, SDL_Color color, bool isFill = false) const;
	void drawSprite(const Sprite& sprite, SDL_Rect bounds) const;
	void drawText(const std::string& text, int x, int y, SDL_Color color) const;

private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	TTF_Font* font;

	mutable std::unordered_map<std::string, SDL_Texture*> textureCache;
};
