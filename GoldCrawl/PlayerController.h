#pragma once

#include "Prelude.h"
#include "Input.h"
#include "Player.h"

class PlayerController
{
public:
	PlayerController(std::unique_ptr<Player> player) :
		player(std::move(player))
	{
	}

	virtual void handleInput(const Input& input) {}
	virtual void update(Uint64 deltaMillis) {}

	Player* getPlayer() const { return player.get(); }

private:
	std::unique_ptr<Player> player;
};
