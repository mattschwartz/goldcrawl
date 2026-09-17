#include "WorldPlayerController.h"
#include "Broom.h"

// pixels per second
constexpr auto SPEED = 64;
// slow down the diagonal movement
constexpr auto DIAGONAL_SPEED = 64 * 0.75;

WorldPlayerController::WorldPlayerController(std::unique_ptr<Player> player, std::unique_ptr<Map> currentMap) :
	PlayerController(std::move(player)),
	currentMap(std::move(currentMap)),
	mapOffset(),
	transitioningToMapOffset(),
	sceneTransitioning(false),
	transitionDurationMillis(0)
{
}

Tile* WorldPlayerController::getTargetedTile() const
{
	if (auto lock = targetedTile.lock())
	{
		return lock.get();
	}
	return nullptr;
}

void WorldPlayerController::handleInput(const Input& input)
{
	if (sceneTransitioning) return;

	Vector playerDirection{ 0,0 };

	if (input.isBindingDown(KeyBinding::Down))
	{
		playerDirection.y += 1;
	}
	if (input.isBindingDown(KeyBinding::Up))
	{
		playerDirection.y += -1;
	}
	if (input.isBindingDown(KeyBinding::Left))
	{
		playerDirection.x += -1;
	}
	if (input.isBindingDown(KeyBinding::Right))
	{
		playerDirection.x += 1;
	}
	if (input.isBindingPressed(KeyBinding::B))
	{
		getPlayer()->getBroom()->sweepBroom(getPlayer()->getPosition(), getMap());
	}
	if (input.isBindingPressed(KeyBinding::A))
	{
		if (auto lock = targetedTile.lock())
		{
			lock->interact();
		}
	}

	getPlayer()->setDirection(playerDirection);
}

void WorldPlayerController::update(Uint64 deltaMillis)
{
	if (sceneTransitioning)
	{
		float deltaSec = deltaMillis / 1000.0 + 0.00001;
		if (mapOffset.x < transitioningToMapOffset.x)
		{
			mapOffset.x += SCREEN_WIDTH * deltaSec;
			// overshoot
			if (mapOffset.x > transitioningToMapOffset.x)
			{
				mapOffset.x = transitioningToMapOffset.x;
				transitionDurationMillis = 0;
			}
		}
		else if (mapOffset.x > transitioningToMapOffset.x)
		{
			mapOffset.x -= SCREEN_WIDTH * deltaSec;
			// overshoot
			if (mapOffset.x < transitioningToMapOffset.x)
			{
				mapOffset.x = transitioningToMapOffset.x;
				transitionDurationMillis = 0;
			}
		}
		if (mapOffset.y < transitioningToMapOffset.y)
		{
			mapOffset.y += SCREEN_HEIGHT * deltaSec;
			// overshoot
			if (mapOffset.y > transitioningToMapOffset.y)
			{
				mapOffset.y = transitioningToMapOffset.y;
				transitionDurationMillis = 0;
			}
		}
		else if (mapOffset.y > transitioningToMapOffset.y)
		{
			mapOffset.y -= SCREEN_HEIGHT * deltaSec;
			// overshoot
			if (mapOffset.y < transitioningToMapOffset.y)
			{
				mapOffset.y = transitioningToMapOffset.y;
				transitionDurationMillis = 0;
			}
		}

		transitionDurationMillis -= deltaMillis;
		if (transitionDurationMillis <= 0)
		{
			sceneTransitioning = false;
			mapOffset.x = transitioningToMapOffset.x;
			mapOffset.y = transitioningToMapOffset.y;
		}
		return;
	}

	auto position = getPlayer()->getPosition();
	auto direction = getPlayer()->getDirection();

	double sec = deltaMillis / 1000.0 + 0.00001;
	double speed = (direction.x != 0 && direction.y != 0) ? DIAGONAL_SPEED : SPEED;

	Vector newPosition{ 0,0 };
	newPosition.x = position.x + direction.x * sec * speed;
	newPosition.y = position.y + direction.y * sec * speed;

	getPlayer()->update(deltaMillis);
	getMap()->update(deltaMillis);

	testInteractables();

	// can't move through walls
	if (!canMove(newPosition)) return;

	// test whether the new position would trigger a screen scroll 
	if (newPosition.x < mapOffset.x)
	{
		sceneTransitioning = true;
		newPosition.x -= TILE_SIZE;
		transitioningToMapOffset.x = mapOffset.x - SCREEN_WIDTH;
	}
	if (newPosition.x + TILE_SIZE > mapOffset.x + SCREEN_WIDTH)
	{
		sceneTransitioning = true;
		newPosition.x += TILE_SIZE;
		transitioningToMapOffset.x = mapOffset.x + SCREEN_WIDTH;
	}
	if (newPosition.y < mapOffset.y)
	{
		sceneTransitioning = true;
		newPosition.y -= TILE_SIZE;
		transitioningToMapOffset.y = mapOffset.y - SCREEN_HEIGHT + TILE_SIZE;
	}
	if (newPosition.y > mapOffset.y + SCREEN_HEIGHT - TILE_SIZE)
	{
		sceneTransitioning = true;
		transitioningToMapOffset.y = mapOffset.y + SCREEN_HEIGHT - TILE_SIZE;
	}
	if (sceneTransitioning)
	{
		transitionDurationMillis = TRANSITION_DURATION;
	}

	getPlayer()->setPosition(newPosition);
}

bool WorldPlayerController::canMove(Vector& newPosition) const
{
	int x = std::floor(getPlayer()->getPosition().x / TILE_SIZE);
	int y = std::floor(getPlayer()->getPosition().y / TILE_SIZE);

	// stay with me...
	std::vector<SDL_Rect> collidingBounds;
	// build a 3x3 grid around where the player is trying to move
	// and construct a virtual room
	for (int i = -1; i <= 1; ++i)
	{
		for (int j = -1; j <= 1; ++j)
		{
			if (getMap()->hasCollision(x + i, y + j))
			{
				collidingBounds.push_back(SDL_Rect{
					(x + i) * TILE_SIZE, (y + j) * TILE_SIZE,
					TILE_SIZE, TILE_SIZE
					});
			}
		}
	}

	// http://tutorialedge.net/gamedev/aabb-collision-detection-tutorial/
	// making the player slightly smaller so that it doesn't get clipped on
	// exact pixels
	const auto isCollision = [](float x, float y, SDL_Rect bounds) -> bool {
		return (x + 1) < bounds.x + bounds.w &&
			(x - 2 + TILE_SIZE) > bounds.x &&
			(y + 1) < bounds.y + bounds.h &&
			(y - 2 + TILE_SIZE) > bounds.y;
		};
	const auto anyCollision = [collidingBounds, isCollision](Vector position) -> bool {
		for (auto& bounds : collidingBounds)
		{
			if (isCollision(position.x, position.y, bounds))
			{
				return true;
			}
		}
		return false;
		};

	auto dir = getPlayer()->getDirection();

	if (!anyCollision(newPosition)) return true;
	if (dir.x != 0 && dir.y != 0)
	{
		if (!anyCollision(Vector{ newPosition.x, getPlayer()->getPosition().y }))
		{
			newPosition.y = getPlayer()->getPosition().y;
			return true;
		}
		if (!anyCollision(Vector{ getPlayer()->getPosition().x, newPosition.y }))
		{
			newPosition.x = getPlayer()->getPosition().x;
			return true;
		}
	}

	return false;
}

bool WorldPlayerController::testInteractables()
{
	auto player = getPlayer();
	auto pos = player->getPosition();
	auto facing = player->getFacing();
	auto map = getMap();

	// normalized to where the player is on the map grid
	int mapX = std::round(pos.x / TILE_SIZE);
	int mapY = std::round(pos.y / TILE_SIZE);

	if (auto it = map->getInteractable(mapX, mapY))
	{
		targetedTile = it;
		return true;
	}

	// looking right
	if (facing.x > 0)
	{
		if (auto it = map->getInteractable(mapX + 1, mapY))
		{
			targetedTile = it;
			return true;
		}
		if (auto it = map->getInteractable(mapX + 1, mapY - 1))
		{
			targetedTile = it;
			return true;
		}
		if (auto it = map->getInteractable(mapX + 1, mapY + 1))
		{
			targetedTile = it;
			return true;
		}
	}
	// looking left
	else if (facing.x < 0)
	{
		if (auto it = map->getInteractable(mapX - 1, mapY))
		{
			targetedTile = it;
			return true;
		}
		if (auto it = map->getInteractable(mapX - 1, mapY - 1))
		{
			targetedTile = it;
			return true;
		}
		if (auto it = map->getInteractable(mapX - 1, mapY + 1))
		{
			targetedTile = it;
			return true;
		}
	}
	// looking up
	else if (facing.y < 0)
	{
		if (auto it = map->getInteractable(mapX, mapY - 1))
		{
			targetedTile = it;
			return true;
		}
		if (auto it = map->getInteractable(mapX - 1, mapY - 1))
		{
			targetedTile = it;
			return true;
		}
		if (auto it = map->getInteractable(mapX + 1, mapY - 1))
		{
			targetedTile = it;
			return true;
		}
	}
	// looking down
	else if (facing.y > 0)
	{
		if (auto it = map->getInteractable(mapX, mapY + 1))
		{
			targetedTile = it;
			return true;
		}
		if (auto it = map->getInteractable(mapX - 1, mapY + 1))
		{
			targetedTile = it;
			return true;
		}
		if (auto it = map->getInteractable(mapX + 1, mapY + 1))
		{
			targetedTile = it;
			return true;
		}
	}

	targetedTile.reset();
	return false;
}
