#include "Prelude.h"
#include <SDL_image.h>
#include "SDLContext.h"
#include "Renderer.h"
#include "Director.h"
#include "Input.h"

#define TITLE "Gold Crawl"
// Rule: gameboy base resolution, we'll scale up in post
#define WIDTH 160
#define HEIGHT 144
#define SCALE 4

static bool tick(Director& director, Renderer& renderer)
{
	auto& input = Input::only();
	if (input.update() == InputUpdateResults::Quit)
	{
		return false; // quit
	}

	renderer.clear();

	director.update();
	director.handleInput();
	director.render(renderer);

	return true;
}

// fun fact: SDL requires this exact header
int main(int argc, char* args[])
{
	try
	{
		// init SDL
		// its destructor forces cleanup automatically when it falls
		// out of scope
		bacon::SDLContext ctx;
		Director director;
		Renderer renderer(TITLE, WIDTH, HEIGHT, SCALE);

		// for when we build for web using emscripten, which doesn't like infinite loops
#ifdef __EMSCRIPTEN__
#else
		while (1)
		{
			// game loop
			if (!tick(director, renderer)) break;
		}
#endif

	}
	catch (const std::exception& e)
	{
		SDL_LogError(0, "Exception occurred: %s", e.what());
	}
	catch (...)
	{
		SDL_LogError(0, "Something bad happened, chief");
	}

	return 0;
}
