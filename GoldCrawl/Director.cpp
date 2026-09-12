#include "Director.h"

void Director::update()
{
	// tick the game
}

void Director::handleInput(Input& input)
{
	// todo - process input in scene
}

void Director::render(const Renderer& renderer)
{
	renderer.drawText("Hello world", 0, 0, { 255,255,255,255 });
	// todo - invoke render on scene
	renderer.render();
}
