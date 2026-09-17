#include "Tile.h"

Tile::Tile(const std::string& staticSpriteFilepath) :
	sprite(Sprite{ staticSpriteFilepath }),
	shouldRemove(false),
	maxHealth(),
	currentHealth(),
	collision(false),
	animation(nullptr)
{
}

Tile::Tile(std::unique_ptr<SpriteAnimation> animation) :
	sprite(std::nullopt),
	shouldRemove(false),
	maxHealth(),
	currentHealth(),
	collision(false),
	animation(std::move(animation))
{
}

void Tile::takeDamage(float cleanDamage)
{
	currentHealth = std::max(currentHealth - cleanDamage, 0.f);
	shouldRemove = currentHealth == 0.0;
}

const Sprite& Tile::getSprite() const
{
	if (sprite.has_value())
	{
		return *sprite;
	}

	return animation->getSprite();
}

void Tile::update(Uint64 delta)
{
	// todo - animations and such
}
