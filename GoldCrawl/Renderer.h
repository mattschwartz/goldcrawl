#pragma once

#include "Prelude.h"

class Renderer
{
public:
	Renderer(const std::string& title, const int width, const int height, const int scale);
	~Renderer();

	void clear();
	void render() const;
private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;
};
