#include "Broom.h"
#include "GameManager.h"

// todo - balance
constexpr float CLEAN_DAMAGE = 8;

Broom::Broom() :
	renderPosition({ 0,0,48,48 }),
	sweepAnimation(std::make_unique<SpriteAnimation>("Sprites/circle_sweep.json", "", false)),
	renderPositionLarge({ 0,0,80,80 }),
	sweepAnimationLarge(std::make_unique<SpriteAnimation>("Sprites/circle_sweep_large.json", "", false))
{
	sweepAnimation->stop();
	sweepAnimationLarge->stop();
}

void Broom::sweepBroom(Vector center, Map* map)
{
	auto& gm = GameManager::only();
	SpriteAnimation* animation;
	int radius;
	if (gm.isUpgradeUnlocked(PlayerUpgrade::DoubleRadius))
	{
		radius = 2;
		animation = sweepAnimationLarge.get();
		renderPositionLarge.x = (int)center.x - TILE_SIZE * 2;
		renderPositionLarge.y = (int)center.y - TILE_SIZE * 2;
	}
	else
	{
		radius = 1;
		animation = sweepAnimation.get();
		renderPosition.x = (int)center.x - TILE_SIZE;
		renderPosition.y = (int)center.y - TILE_SIZE;
	}

	int damage;
	if (gm.isUpgradeUnlocked(PlayerUpgrade::DoubleCleanliness))
	{
		damage = CLEAN_DAMAGE * 2;
	}
	else
	{
		damage = CLEAN_DAMAGE;
	}

	if (!animation->isComplete()) return;

	// get the grid coordinates
	int x = std::floor(center.x / TILE_SIZE);
	int y = std::floor(center.y / TILE_SIZE);

	// get tiles around the center from the map
	for (int i = -radius; i <= radius; ++i)
	{
		for (int j = -radius; j <= radius; ++j)
		{
			auto tiles = map->getTilesAt(x + i, y + j);
			for (auto& t : tiles)
			{
				if (t->isCleanable() && t->getMaxHealth() > 0)
				{
					t->takeDamage(damage);
					// only report clean when the tile is fully clean
					if (t->getCurrentHealth() <= 0)
					{
						map->addCleanDamage(t->getMaxHealth());
					}
				}
			}
		}
	}

	animation->setCycle(""); // reset the animation
}

void Broom::update(Uint64 deltaMillis)
{
	auto& gm = GameManager::only();
	if (gm.isUpgradeUnlocked(PlayerUpgrade::DoubleRadius))
	{
		sweepAnimationLarge->update(deltaMillis);
	}
	else
	{
		sweepAnimation->update(deltaMillis);
	}
}

SDL_Rect Broom::getPosition() const
{
	auto& gm = GameManager::only();
	if (gm.isUpgradeUnlocked(PlayerUpgrade::DoubleRadius))
	{
		return renderPositionLarge;
	}
	else
	{
		return renderPosition;
	}
}

const Sprite& Broom::getSprite() const
{
	auto& gm = GameManager::only();
	if (gm.isUpgradeUnlocked(PlayerUpgrade::DoubleRadius))
	{
		return sweepAnimationLarge->getSprite();
	}
	else
	{
		return sweepAnimation->getSprite();
	}
}
