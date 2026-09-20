#include "UpgradeTile.h"
#include "Player.h"
#include "GameManager.h"

void UpgradeTile::interact(Player& player)
{
	GameManager::only().openShop("Do you want to buy this thing", upgrade, upgradeCost);
}
