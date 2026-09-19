#include "TrapTile.h"
#include "AnimationManager.h"

void TrapTile::interact(class Player&)
{
	if (trapSet) return;

	// play "set trap" sound effect
	// play "set trap" animation effect
	AnimationManager::only().addOneShot("Sprites/set_trap_animation.json", {
		position.x * TILE_SIZE, position.y * TILE_SIZE, 16, 16});

	// reset the sprite
	animation.reset();
	sprite = Sprite(setTrapSprite);
	trapSet = true;
}
