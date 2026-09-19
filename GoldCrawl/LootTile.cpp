#include "LootTile.h"
#include "AnimationManager.h"

void LootTile::interact()
{
	// play "set trap" sound effect
	// play "set trap" animation effect
	AnimationManager::only().addOneShot("Sprites/coin.json", {
		position.x * TILE_SIZE, position.y * TILE_SIZE, 16, 16 });

	// reset the sprite
	animation.reset();
	shouldRemove = true;
}
