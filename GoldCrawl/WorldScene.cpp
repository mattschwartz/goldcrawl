#include "WorldScene.h"
#include "AnimationManager.h"
#include <format>
#include "Color.h"

WorldScene::WorldScene(std::unique_ptr<Map> currentMap) : isFadingOutScene(false), isFadingInScene(false)
{
	controller = std::make_unique<WorldPlayerController>(
		std::make_unique<Player>(),
		std::move(currentMap));
	transitionScene = std::make_unique<MapTransitionScene>();
}

void WorldScene::handleInput(const Input& input)
{
	controller->handleInput(input);
}

void WorldScene::update(Uint64 delta)
{
	if (isFadingOutScene)
	{
		transitionScene->update(delta);
		if (transitionScene->isComplete())
		{
			controller->switchToLoadingMap();
			transitionScene->startFadeIn();
			isFadingOutScene = false;
			isFadingInScene = true;
		}
		return;
	}
	else if (isFadingInScene)
	{
		transitionScene->update(delta);
		if (transitionScene->isComplete())
		{
			isFadingInScene = false;
		}
		return;
	}

	controller->update(delta);
	// new map to load, start sequence
	if (controller->isLoadingNewMap)
	{
		SDL_Log("Loading new map");
		transitionScene->startFadeOut();
		isFadingOutScene = true;
	}
	AnimationManager::only().update(delta);
}

void WorldScene::render(const Renderer& renderer) const
{
	renderMap(renderer);
	renderPlayer(renderer);
	renderToolbar(renderer);
	AnimationManager::only().render(renderer, controller->getMapOffset());

	if (isFadingOutScene || isFadingInScene)
	{
		transitionScene->render(renderer);
	}
}

void WorldScene::renderMap(const Renderer& renderer) const
{
	auto offs = controller->getMapOffset();
	// hack
	int promptX = -1, promptY = -1;
	bool renderPrompt = false;
	for (auto& layer : SortedTileLayers)
	{
		for (auto& [position, tile] : controller->getMap()->getTiles(layer))
		{
			int x = (position.x * TILE_SIZE) - (int)offs.x;
			int y = (position.y * TILE_SIZE) - (int)offs.y;

			// only render what's in view, plus the transition scenes
			if (x < -TILE_SIZE || x > SCREEN_WIDTH + TILE_SIZE
				|| y < -TILE_SIZE || y > SCREEN_HEIGHT + TILE_SIZE)
			{
				continue;
			}

			if (!tile->hasSprite()) continue;

			renderer.drawSprite(tile->getSprite(), {
				x,
				y,
				TILE_SIZE,
				TILE_SIZE });
			if (tile->isCleanable())
			{
				if (controller->shouldShowDirtMarkers())
				{
					static auto markerSprite = Sprite{ "Sprites/dirty_tile_marker.png" };
					renderer.drawSprite(markerSprite, { x, y, TILE_SIZE, TILE_SIZE });
				}
			}
			if (tile->getCurrentHealth() != tile->getMaxHealth())
			{
				renderer.drawBox({ x, y + TILE_SIZE - 2, TILE_SIZE, 2 }, colors::darkest, true);
				renderer.drawBox({ x, y + TILE_SIZE - 2, (int)(TILE_SIZE * tile->getCurrentHealth() / tile->getMaxHealth()), 2 }, colors::base, true);
			}

			if (controller->getTargetedTile() && controller->getTargetedTile() == tile.get())
			{
				renderPrompt = true;
				promptX = x;
				promptY = y;
			}
		}
	}

	// hack just to make it render on top without dealing with
	// a bunch of nonsense
	if (renderPrompt)
	{
		static auto promptButton = Sprite{ "Sprites/interact_prompt_button.png", SDL_Rect{0,0,16,20} };
		renderer.drawSprite(promptButton, {
			promptX, promptY - 16,
			TILE_SIZE, 20
			});
	}
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

	renderer.drawSprite(player->getSprite(), { (int)x, (int)y, TILE_SIZE, TILE_SIZE });

	auto broomPosition = player->getBroom()->getPosition();
	broomPosition.x -= controller->getMapOffset().x;
	broomPosition.y -= controller->getMapOffset().y;

	renderer.drawSprite(player->getBroom()->getSprite(), broomPosition);
}

void WorldScene::renderToolbar(const Renderer& renderer) const
{
	auto map = controller->getMap();
	float clean = map->getCleanDamage();
	float dirt = map->getTotalDirtLevel();

	float cleanPercent = dirt == 0 ? 1 : std::min(clean / dirt, 1.0f);
	int numCleanGems = (int)(cleanPercent * 10.0f);

	int x = 0, y = 144 - 16;
	static auto toolbarSprite = Sprite{ "Sprites/gui_toolbar.png", {0, 0, SCREEN_WIDTH, TILE_SIZE} };
	static auto gemSprite = Sprite{ "Sprites/clean_gem.png" };

	renderer.drawSprite(toolbarSprite, { x, y, SCREEN_WIDTH, TILE_SIZE });
	// https://stackoverflow.com/questions/530614/print-leading-zeros-with-c-output-operator
	std::string coinText = std::format("{:05}", controller->getPlayer()->getGold());
	renderer.drawText(coinText, x + 25, y + 1, colors::darkest);

	int gemX = 100, gemY = y + 6;
	for (int i = 0; i < numCleanGems; ++i, gemX += 6)
	{
		renderer.drawSprite(gemSprite, { gemX, gemY, 5, 9 });
	}
}
