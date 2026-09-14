#include "WorldPlayerController.h"

// pixels per second
constexpr auto SPEED = 64;
// slow down the diagonal movement
constexpr auto DIAGONAL_SPEED = 64 * 0.75;

WorldPlayerController::WorldPlayerController(std::unique_ptr<Player> player, std::unique_ptr<Map> currentMap) :
	PlayerController(std::move(player)),
	currentMap(std::move(currentMap))
{
}

void WorldPlayerController::handleInput(const Input& input)
{
	Vector playerDirection{ 0,0 };

	if (input.isBindingDown(KeyBinding::Down))
	{
		playerDirection.y = 1;
	}
	if (input.isBindingDown(KeyBinding::Up))
	{
		playerDirection.y = -1;
	}
	if (input.isBindingDown(KeyBinding::Left))
	{
		playerDirection.x = -1;
	}
	if (input.isBindingDown(KeyBinding::Right))
	{
		playerDirection.x = 1;
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

	// todo - test new position against physics calculation

	// test whether the new position would trigger a screen scroll 
	if (newPosition.x < 0)
	{
		SDL_Log("Scrolling right... from the left.. to the right");
	}
	if (newPosition.x + TILE_SIZE > SCREEN_WIDTH)
	{
		SDL_Log("Scrolling left... from the right... to the left");
	}
	if (newPosition.y < 0)
	{
		SDL_Log("scrolling ^");
	}
	if (newPosition.y + TILE_SIZE > SCREEN_HEIGHT)
	{
		SDL_Log("Scrolling down");
	}

	getPlayer()->setPosition(newPosition);
}
