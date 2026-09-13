#include "WorldScene.h"

WorldScene::WorldScene()
{
	controller = std::make_unique<WorldPlayerController>(
		std::make_unique<Player>());
}

void WorldScene::handleInput(const Input& input)
{
	controller->handleInput(input);
}

void WorldScene::update(Uint64 delta)
{
	// todo - update the world
	controller->update(delta);
}

void WorldScene::render(const Renderer& renderer) const
{
	auto player = controller->getPlayer();
	float x = player->getPosition().x;
	float y = player->getPosition().y;

	renderer.drawSprite("Sprites/charlie.png", { (int)x, (int)y, 16, 16 });
	renderer.drawBox({ 50,50,16,16 }, { 255,0,0,255 });
	renderer.drawText("Hello world", 0, 0, { 255,255,255,255 });
}
