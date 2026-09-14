#include "WorldScene.h"
#include "Color.h"

WorldScene::WorldScene(std::unique_ptr<Map> currentMap)
{
	controller = std::make_unique<WorldPlayerController>(
		std::make_unique<Player>(),
		std::move(currentMap));
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
	renderMap(renderer);
	renderPlayer(renderer);
}

void WorldScene::renderMap(const Renderer& renderer) const
{
	// todo - get offset based on player? controller? updates?
	//for (auto& tile : controller->getMap()->getTiles())
	//{
	//	renderer.drawSprite(tile->getSprite(), {});
	//}
}

void WorldScene::renderPlayer(const Renderer& renderer) const
{
	auto player = controller->getPlayer();
	auto pos = player->getPosition();
	auto dir = player->getDirection();
	auto reticle = pos + Vector{ dir.x * TILE_SIZE, dir.y * TILE_SIZE };

	float x = player->getPosition().x - controller->getMapOffset().x;
	float y = player->getPosition().y - controller->getMapOffset().y;

	reticle.x -= controller->getMapOffset().x;
	reticle.y -= controller->getMapOffset().y;

	renderer.drawSprite("Sprites/charlie.png", { (int)x, (int)y, TILE_SIZE, TILE_SIZE });
	renderer.drawBox({ (int)reticle.x, (int)reticle.y, TILE_SIZE, TILE_SIZE }, colors::base);
	renderer.drawText("Hello world", 0, 0, colors::highlight);
}
