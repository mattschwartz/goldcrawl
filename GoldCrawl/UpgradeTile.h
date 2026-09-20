#pragma once

#include "Tile.h"
#include "PlayerUpgrades.h"

class UpgradeTile : public Tile
{
public:
	UpgradeTile(PlayerUpgrade upgrade, int upgradeCost) : upgrade(upgrade), upgradeCost(upgradeCost) {}

	bool isInteractable() const { return true; }
	void interact(class Player& player) override;

private:
	PlayerUpgrade upgrade;
	int upgradeCost;
};
