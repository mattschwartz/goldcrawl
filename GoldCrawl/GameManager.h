#pragma once

#include "Prelude.h"
#include "DialogController.h"
#include "ShopController.h"
#include "PlayerUpgrades.h"

class GameManager
{
public:
	static GameManager& only();

	int getGold() const { return gold; }
	void addGold(int amt) { gold += amt; }

	DialogController& getDialogController() const;
	ShopController& getShopController() const;

	bool isUpgradeUnlocked(PlayerUpgrade upgrade) const;
	void unlockUpgrade(PlayerUpgrade upgrade);

	bool isInShop() const;
	void openShop(const std::string& prompt, PlayerUpgrade upgrade, int cost);
	void closeShop();

	bool isInDialog() const;
	void openDialog(const std::string& dialogText);
	void closeDialog();

private:
	int gold;

	bool inShop;
	bool inDialog;
	std::unique_ptr<DialogController> dialogController;
	std::unique_ptr<ShopController> shopController;
	// if true, upgrade is unlocked
	std::unordered_map<PlayerUpgrade, bool> playerUpgrades;

	GameManager();
};
