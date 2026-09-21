#include "Prelude.h"
#include <SDL_image.h>
#include "SDLContext.h"
#include "Renderer.h"
#include "Director.h"
#include "Input.h"
#include "WorldScene.h"
#include "Map.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

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

static std::shared_ptr<bacon::SDLContext> _context;
static std::shared_ptr<Director> _director;
static std::shared_ptr<Input> _input;
static std::shared_ptr<Renderer> _renderer;

#ifdef __EMSCRIPTEN__
static void webTick()
{
	if (!tick(*_director, *_input, *_renderer))
	{
		emscripten_cancel_main_loop();
	}
}
#endif

// fun fact: SDL requires this exact header
int main(int argc, char* args[])
{
	try
	{
		// init SDL
		// its destructor forces cleanup automatically when it falls
		// out of scope
		_context = std::make_shared<bacon::SDLContext>();
		_director = std::make_shared<Director>(std::make_unique<WorldScene>()); // WorldScene starting scene
		_input = std::make_shared<Input>();
		_renderer = std::make_shared<Renderer>(TITLE, SCREEN_WIDTH, SCREEN_HEIGHT, SCALE);

		// for when we build for web using emscripten, which doesn't like infinite loops
#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop(webTick, 0, 0);
#else
		while (1)
		{
			// game loop
			if (!tick(*_director, *_input, *_renderer)) break;
		}
		_renderer.reset();
		_input.reset();
		_director.reset();
		_context.reset();
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
