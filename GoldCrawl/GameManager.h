#pragma once

#include "Prelude.h"
#include "DialogController.h"
#include "PlayerUpgrades.h"

class GameManager
{
public:
	static GameManager& only();

	DialogController& getDialogController() const;

	bool isUpgradeUnlocked(PlayerUpgrade upgrade) const;
	void unlockUpgrade(PlayerUpgrade upgrade);

	bool isInDialog() const;
	void openDialog(const std::string& dialogText);
	void closeDialog();

private:
	bool inDialog;
	std::unique_ptr<DialogController> dialogController;
	// if true, upgrade is unlocked
	std::unordered_map<PlayerUpgrade, bool> playerUpgrades;

	GameManager();
};
