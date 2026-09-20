#pragma once

#include "Tile.h"
#include "PlayerUpgrades.h"

class UpgradeTile : public Tile
{
public:
	UpgradeTile(PlayerUpgrade upgrade, int upgradeCost) : consumed(false), upgrade(upgrade), upgradeCost(upgradeCost) {}

	bool isInteractable() const { return !consumed; }
	void interact(class Player& player) override;

private:
	bool consumed;
	PlayerUpgrade upgrade;
	int upgradeCost;
};
