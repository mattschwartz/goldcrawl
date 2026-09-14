#include "Renderer.h"
#include <format>
#include <SDL_image.h>
#include "SDLContext.h"
#include "FileSystem.h"
#include "Color.h"

constexpr auto FONT_SIZE = 12;

Renderer::Renderer(const std::string& title, const int width, const int height, const int scale)
{
	_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * scale, height * scale, SDL_WINDOW_OPENGL);
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

	if (_renderer == nullptr)
	{
		throw bacon::SDLError(std::format("failed to create renderer: {}", SDL_GetError()));
	}

	SDL_SetRenderDrawColor(_renderer, colors::darkest.r, colors::darkest.g, colors::darkest.b, colors::darkest.a); // dark from palette
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
	// todo - clear cache
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

void Renderer::drawBox(SDL_Rect bounds, SDL_Color color, bool isFill) const
{
	SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
	if (isFill)
	{
		SDL_RenderFillRect(_renderer, &bounds);
	}
	else
	{
		SDL_RenderDrawRect(_renderer, &bounds);
	}
	SDL_SetRenderDrawColor(_renderer, colors::darkest.r, colors::darkest.g, colors::darkest.b, colors::darkest.a);
}

void Renderer::drawSprite(const std::string& sprite, SDL_Rect bounds) const
{
	std::string cacheKey = sprite;

	SDL_Texture* texture = nullptr;

	if (auto it = textureCache.find(cacheKey); it != textureCache.end())
	{
		texture = it->second;
	}
	else
	{
		SDL_Surface* srf = IMG_Load(sprite.c_str());
		texture = SDL_CreateTextureFromSurface(_renderer, srf);
		SDL_FreeSurface(srf);
		textureCache[cacheKey] = texture;
		if (!texture)
		{
			SDL_LogError(0, "Sprite %s does not exist", sprite.c_str());
			return;
		}
	}

	SDL_RenderCopy(_renderer, texture, NULL, &bounds);
}

void Renderer::drawText(const std::string& text, int x, int y, SDL_Color color) const
{
	std::string cacheKey = std::format("{}.{}.{}.{}.{}", color.r, color.g, color.b, color.a, text);

	SDL_Texture* texture = nullptr;

	if (auto it = textureCache.find(cacheKey); it != textureCache.end())
	{
		texture = it->second;
	}
	else
	{
		SDL_Surface* srf = TTF_RenderUTF8_Solid(font, text.c_str(), color);
		texture = SDL_CreateTextureFromSurface(_renderer, srf);
		SDL_FreeSurface(srf);
		textureCache[cacheKey] = texture;
	}

	int texW, texH;
	SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
	SDL_Rect textureRect{ x, y, texW, texH };
	SDL_RenderCopy(_renderer, texture, NULL, &textureRect);
}

