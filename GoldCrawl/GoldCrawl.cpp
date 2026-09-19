#include "Prelude.h"
#include <SDL_image.h>
#include "SDLContext.h"
#include "Renderer.h"
#include "Director.h"
#include "Input.h"
#include "WorldScene.h"
#include "Map.h"
#include "TiledImporter.h"

#define TITLE "Clean, Crawl & Carry On"
#define SCALE 4

static bool tick(Director& director, Input& input, Renderer& renderer)
{
	if (input.update() == InputUpdateResults::Quit)
	{
		return false; // quit
	}

	renderer.clear();

	director.update();
	director.handleInput(input);
	director.render(renderer);

	return true;
}


// fun fact: SDL requires this exact header
int main(int argc, char* args[])
{
	TiledImporter imp{};
	auto starterMap = imp.parseTiledMap("Maps/overworld.tmj");

	try
	{
		// init SDL
		// its destructor forces cleanup automatically when it falls
		// out of scope
		bacon::SDLContext ctx;
		Director director(std::make_unique<WorldScene>(std::move(starterMap))); // WorldScene starting scene
		Input input;
		Renderer renderer(TITLE, SCREEN_WIDTH, SCREEN_HEIGHT, SCALE);

		// for when we build for web using emscripten, which doesn't like infinite loops
#ifdef __EMSCRIPTEN__
#else
		while (1)
		{
			// game loop
			if (!tick(director, input, renderer)) break;
		}
#endif

	}
	catch (const std::exception& e)
	{
		SDL_LogError(0, "Exception occurred: %s", e.what());
		return 1;
	}
	catch (...)
	{
		SDL_LogError(0, "Something bad happened, chief");
		return 1;
	}

	return 0;
}
