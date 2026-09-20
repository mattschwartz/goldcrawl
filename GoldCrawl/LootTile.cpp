#include "LootTile.h"
#include "AnimationManager.h"
#include "Player.h"
#include "GameManager.h"

void LootTile::interact(Player& player)
{
	if (goldAmount > 0)
	{
		GameManager::only().addGold(goldAmount);

		// play "set trap" sound effect
		// play "set trap" animation effect
		AnimationManager::only().addOneShot("Sprites/coin.json", {
			position.x * TILE_SIZE, position.y * TILE_SIZE, 16, 16 });

		// reset the sprite
		animation.reset();
	}
	else
	{
		AnimationManager::only().addOneShot("Sprites/set_trap_animation.json", {
			position.x * TILE_SIZE, position.y * TILE_SIZE, 16, 16 });
		animation.reset();
	}
	shouldRemove = true;
}
