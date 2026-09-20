#include "GameManager.h"

GameManager::GameManager() :
	inShop(false),
	inDialog(false),
	dialogController(std::make_unique<DialogController>()),
	shopController(std::make_unique<ShopController>())
{
}

GameManager& GameManager::only()
{
	static GameManager instance;
	return instance;
}

DialogController& GameManager::getDialogController() const
{
	return *dialogController;
}

ShopController& GameManager::getShopController() const
{
	return *shopController;
}

bool GameManager::isUpgradeUnlocked(PlayerUpgrade upgrade) const
{
	if (auto it = playerUpgrades.find(upgrade); it != playerUpgrades.end())
	{
		return it->second;
	}
	return false;
}

void GameManager::unlockUpgrade(PlayerUpgrade upgrade)
{
	playerUpgrades.emplace(upgrade, true);
}

bool GameManager::isInShop() const
{
	return inShop;
}

void GameManager::openShop(const std::string& prompt, PlayerUpgrade upgrade, int cost)
{
	inShop = true;
	shopController->openShop(prompt, upgrade, cost);
}

void GameManager::closeShop()
{
	inShop = false;
}

bool GameManager::isInDialog() const
{
	return inDialog;
}

void GameManager::openDialog(const std::string& dialogText)
{
	inDialog = true;
	dialogController->startDialog(dialogText);
}

void GameManager::closeDialog()
{
	inDialog = false;
}
