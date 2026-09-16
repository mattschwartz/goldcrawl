#include "WorldPlayerController.h"

// pixels per second
constexpr auto SPEED = 64;
// slow down the diagonal movement
constexpr auto DIAGONAL_SPEED = 64 * 0.75;

WorldPlayerController::WorldPlayerController(std::unique_ptr<Player> player, std::unique_ptr<Map> currentMap) :
	PlayerController(std::move(player)),
	currentMap(std::move(currentMap)),
	mapOffset()
{
}

void WorldPlayerController::handleInput(const Input& input)
{
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

	getPlayer()->setDirection(playerDirection);
}

void WorldPlayerController::update(Uint64 deltaMillis)
{
	auto position = getPlayer()->getPosition();
	auto direction = getPlayer()->getDirection();

	double sec = deltaMillis / 1000.0 + 0.00001;
	double speed = (direction.x != 0 && direction.y != 0) ? DIAGONAL_SPEED : SPEED;

	Vector newPosition{ 0,0 };
	newPosition.x = position.x + direction.x * sec * speed;
	newPosition.y = position.y + direction.y * sec * speed;

	// can't move through walls
	if (!canMove(newPosition)) return;

	// test whether the new position would trigger a screen scroll 
	if (newPosition.x < mapOffset.x)
	{
		mapOffset.x -= SCREEN_WIDTH;
	}
	if (newPosition.x + TILE_SIZE > mapOffset.x + SCREEN_WIDTH)
	{
		mapOffset.x += SCREEN_WIDTH;
	}
	if (newPosition.y < mapOffset.y)
	{
		mapOffset.y -= SCREEN_HEIGHT;
	}
	if (newPosition.y + TILE_SIZE > mapOffset.y + SCREEN_HEIGHT)
	{
		mapOffset.y += SCREEN_HEIGHT;
	}

	getPlayer()->setPosition(newPosition);
}

bool WorldPlayerController::canMove(Vector& newPosition) const
{
	int x = newPosition.x;
	int y = newPosition.y;

	auto dir = getPlayer()->getDirection();
	// moving to the right, account for player width
	if (dir.x > 0)
	{
		x += TILE_SIZE - 1; // player width
	}
	if (dir.y > 0)
	{
		y += TILE_SIZE - 1; //player height
	}

	if (getMap()->hasCollision((int)(x / TILE_SIZE), (int)(y / TILE_SIZE)))
	{
		// if we're moving diagonal, try sliding
		if (dir.x != 0 && dir.y != 0)
		{
			if (!getMap()->hasCollision((int)(getPlayer()->getPosition().x) / TILE_SIZE, (int)(y / TILE_SIZE)))
			{
				newPosition.x = getPlayer()->getPosition().x;
				return true;
			}
			if (!getMap()->hasCollision((int)x / TILE_SIZE, (int)((getPlayer()->getPosition().y) / TILE_SIZE)))
			{
				newPosition.y = getPlayer()->getPosition().y;
				return true;
			}
		}

		return false;
	}

	return true;
}
