#include "Broom.h"

Broom::Broom() :
	renderPosition({ 0,0,48,48 }),
	sweepAnimation(std::make_unique<SpriteAnimation>("Sprites/circle_sweep.json", "", false))
{
	sweepAnimation->stop();
}

void Broom::sweepBroom(Vector center, Map* map)
{
	if (!sweepAnimation->isComplete()) return;

	// todo - tidy up nearby debris on hit
	sweepAnimation->setCycle(""); // reset the animation

	renderPosition.x = (int)center.x - TILE_SIZE;
	renderPosition.y = (int)center.y - TILE_SIZE;
}

void Broom::update(Uint64 deltaMillis)
{
	sweepAnimation->update(deltaMillis);
}

const Sprite& Broom::getSprite() const
{
	return sweepAnimation->getSprite();
}
