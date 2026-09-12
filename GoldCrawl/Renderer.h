#pragma once

#include "Prelude.h"
#include <SDL_ttf.h>

class Renderer
{
public:
	Renderer(const std::string& title, const int width, const int height, const int scale);
	~Renderer();

	void clear();
	void render() const;

	void drawBox() const;
	void drawSprite() const;
	void drawText(const std::string& text, int x, int y, SDL_Color color) const;

private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	TTF_Font* font;
};
