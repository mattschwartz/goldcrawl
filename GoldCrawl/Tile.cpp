#include "Tile.h"

Tile::Tile() :
	cleanable(false),
	position(),
	sprite(std::nullopt),
	shouldRemove(false),
	maxHealth(),
	currentHealth(),
	collision(false),
	animation(nullptr)
{
}

float Tile::takeDamage(float cleanDamage)
{
	float previousHealth = currentHealth;
	currentHealth = std::max(currentHealth - cleanDamage, 0.f);
	shouldRemove = currentHealth == 0.0;
	return previousHealth - currentHealth;
}

bool Tile::hasSprite() const
{
	return sprite.has_value() || animation;
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
	if (animation) animation->update(delta);
}
