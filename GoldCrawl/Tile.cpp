#include "Tile.h"

Tile::Tile(const std::string& staticSpriteFilepath) :
	sprite(Sprite{ staticSpriteFilepath }),
	collision(false),
	animation(nullptr)
{
}

Tile::Tile(std::unique_ptr<SpriteAnimation> animation) :
	sprite(std::nullopt),
	collision(false),
	animation(std::move(animation))
{
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
