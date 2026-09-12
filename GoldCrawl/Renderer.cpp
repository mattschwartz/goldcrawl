#include "Renderer.h"
#include <format>
#include "SDLContext.h"
#include "FileSystem.h"

constexpr auto FONT_SIZE = 12;

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

	
	std::string fontFilepath = bacon::fs::resolve("BaconSans.ttf");
	auto font = TTF_OpenFont(fontFilepath.c_str(), FONT_SIZE);
	if (!font)
	{
		std::string msg = std::format("failed to open font: {}", TTF_GetError());
		SDL_LogError(0, msg.c_str());
		throw bacon::SDLError(msg);
	}
	this->font = font;
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

void Renderer::drawText(const std::string& text, int x, int y, SDL_Color color) const
{
	// todo - add cache
	SDL_Surface* srf = TTF_RenderUTF8_Solid(font, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, srf);
	SDL_FreeSurface(srf);

	int texW, texH;
	SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
	SDL_Rect textureRect{ x, y, texW, texH };

	SDL_RenderCopy(_renderer, texture, NULL, &textureRect);
}

