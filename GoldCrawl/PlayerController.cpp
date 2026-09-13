#include "PlayerController.h"

PlayerController::PlayerController(std::unique_ptr<Player> player) :
	player(std::move(player))
{
}
