#pragma once

#include "Tile.h"

class TrapTile : public Tile
{
public:
	TrapTile(const std::string& setTrapSprite) : trapSet(false), setTrapSprite(setTrapSprite) {}

	bool isInteractable() const override { return !trapSet; }

	// will reset the trap
	void interact() override;

private:
	// the sprite to show when the trap is set
	std::string setTrapSprite;
	bool trapSet;
};
