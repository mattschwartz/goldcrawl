#include "WorldPlayerController.h"

// pixels per second
constexpr auto SPEED = 64;
// slow down the diagonal movement
constexpr auto DIAGONAL_SPEED = 64 * 0.75;

WorldPlayerController::WorldPlayerController(std::unique_ptr<Player> player) :
	PlayerController(std::move(player))
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

	// todo - physics calculation
	double sec = deltaMillis / 1000.0 + 0.00001;
	double speed = (direction.x != 0 && direction.y != 0) ? DIAGONAL_SPEED : SPEED;

	position.x += direction.x * sec * speed;
	position.y += direction.y * sec * speed;
	getPlayer()->setPosition(position);
}
