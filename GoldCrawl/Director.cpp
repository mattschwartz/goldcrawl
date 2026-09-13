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
	renderer.drawSprite("Sprites/charlie.png", { 30,30, 16, 16 });
	renderer.drawBox({ 0,0,15,15 }, { 255,0,0,255 });
	renderer.drawText("Hello world", 0, 0, { 255,255,255,255 });
	// todo - invoke render on scene
	renderer.render();
}
