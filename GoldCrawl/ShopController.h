#pragma once

#include "Prelude.h"
#include "Input.h"
#include "PlayerUpgrades.h"
#include "Renderer.h"

class ShopController
{
public:

	void openShop(const std::string& prompt, PlayerUpgrade upgrade, int cost);

	void handleInput(const Input& input);
	void update(Uint64 delta);
	void render(const Renderer& renderer) const;

private:
	std::string prompt;
	PlayerUpgrade upgrade;
	int cost;
	int selection;
	bool notEnoughMoney;
};
