#include "Director.h"
#include "AnimationManager.h"

Director::Director(std::unique_ptr<Scene> startingScene) : 
	scene(std::move(startingScene)),
	lastTicks(SDL_GetTicks64())
{
}

void Director::update()
{
	Uint64 now = SDL_GetTicks64();
	Uint64 delta = now - lastTicks;
	lastTicks = now;

	scene->update(delta);
	AnimationManager::only().update(delta);
}

void Director::handleInput(Input& input)
{
	scene->handleInput(input);
}

void Director::render(const Renderer& renderer)
{
	scene->render(renderer);
	AnimationManager::only().render(renderer);
	renderer.render();
}
