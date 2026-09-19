#include "Broom.h"

// todo - balance
constexpr float CLEAN_DAMAGE = 8;

Broom::Broom() :
	renderPosition({ 0,0,48,48 }),
	sweepAnimation(std::make_unique<SpriteAnimation>("Sprites/circle_sweep.json", "", false))
{
	sweepAnimation->stop();
}

void Broom::sweepBroom(Vector center, Map* map)
{
	if (!sweepAnimation->isComplete()) return;

	// get the grid coordinates
	int x = std::floor(center.x / TILE_SIZE);
	int y = std::floor(center.y / TILE_SIZE);

	// get tiles around the center from the map
	for (int i = -1; i <= 1; ++i)
	{
		for (int j = -1; j <= 1; ++j)
		{
			auto tiles = map->getTilesAt(x + i, y + j);
			for (auto& t : tiles)
			{
				if (t->getMaxHealth() > 0)
				{
					t->takeDamage(CLEAN_DAMAGE);
				}
			}
		}
	}

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
