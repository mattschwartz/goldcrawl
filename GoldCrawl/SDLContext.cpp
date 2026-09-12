#include "SDLContext.h"
#include <format>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

bacon::SDLContext::SDLContext()
{
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
    {
        throw SDLError(std::format("failed to init sdl: {}", SDL_GetError()));
    }

    if (IMG_Init(0) != 0)
    {
        throw SDLError(std::format("failed to init img: {}", IMG_GetError()));
    }

    if (TTF_Init() != 0)
    {
        throw SDLError(std::format("failed to init ttf: {}", TTF_GetError()));
    }
}

bacon::SDLContext::~SDLContext()
{
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}

