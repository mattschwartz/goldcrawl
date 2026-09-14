#include "Tile.h"

Tile::Tile(const std::string& sprite) : sprite(sprite)
{
}

std::string Tile::getSprite() const
{
	return sprite;
}

void Tile::update(float delta)
{
	// todo - animations and such
}
