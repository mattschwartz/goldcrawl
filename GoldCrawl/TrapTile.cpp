#include "TrapTile.h"

void TrapTile::interact()
{
	if (trapSet) return;

	// play "set trap" sound effect
	// play "set trap" animation effect

	// reset the sprite
	animation.reset();
	sprite = Sprite(setTrapSprite);
	trapSet = true;
}
