#pragma once

#include "PlayerController.h"
#include "Vector.h"

class WorldPlayerController : public PlayerController
{
public:
	WorldPlayerController(std::unique_ptr<Player> player);

	void handleInput(const Input& input) override;
	void update(Uint64 deltaMillis) override;
};
