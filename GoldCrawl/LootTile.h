#pragma once

#include "Tile.h"

class LootTile : public Tile
{
public:
	LootTile(int goldAmount) : goldAmount(goldAmount) {}

	bool isInteractable() const override { return true; }
	void interact(class Player& player) override;

private:
	int goldAmount;
};
