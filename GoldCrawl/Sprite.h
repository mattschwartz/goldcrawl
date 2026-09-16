#pragma once

#include <string>
#include <SDL.h>

struct Sprite
{
	std::string filepath;
	SDL_Rect sourceRect;

	Sprite(const std::string& filepath) : filepath(filepath), sourceRect({ 0,0,16,16 }) {}
	Sprite(const std::string& filepath, SDL_Rect src) : filepath(filepath), sourceRect(src) {}
};
