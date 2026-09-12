#include "Renderer.h"
#include <format>
#include "SDLContext.h"

Renderer::Renderer(const std::string& title, const int width, const int height, const int scale)
{
	_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * scale, height * scale, SDL_WINDOW_OPENGL);
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

	if (_renderer == nullptr)
	{
		throw bacon::SDLError(std::format("failed to create renderer: {}", SDL_GetError()));
	}

	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255); // black
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderSetLogicalSize(_renderer, width, height);
	SDL_SetWindowMinimumSize(_window, width * scale, height * scale);
}

Renderer::~Renderer()
{
	SDL_DestroyWindow(_window);
}

void Renderer::clear()
{
	SDL_RenderClear(_renderer);
}

void Renderer::render() const
{
	SDL_RenderPresent(_renderer);
}

