#include "UpgradeTile.h"
#include "Player.h"
#include "GameManager.h"

void UpgradeTile::interact(Player& player)
{
	if (consumed) return;

	if (player.getGold() < upgradeCost)
	{
		// todo - play no sound
		SDL_LogError(0, "cannot afford");
		return;
	}

	player.addGold(-upgradeCost);
	GameManager::only().unlockUpgrade(upgrade);
	consumed = true;
}
