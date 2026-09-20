#include "LootTile.h"
#include "AnimationManager.h"
#include "Player.h"
#include "GameManager.h"

void LootTile::interact(Player& player)
{
	GameManager::only().addGold(123);

	// play "set trap" sound effect
	// play "set trap" animation effect
	AnimationManager::only().addOneShot("Sprites/coin.json", {
		position.x * TILE_SIZE, position.y * TILE_SIZE, 16, 16 });

	// reset the sprite
	animation.reset();
	shouldRemove = true;
}
