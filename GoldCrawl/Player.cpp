#include "Player.h"

Player::Player() :
	position({ 16, 16 }),
	direction(),
	sprite(Sprite{ "Sprites/charlie.png" }),
	animation(std::make_unique<SpriteAnimation>("Sprites/charlie.json"))
{
}

const Sprite& Player::getSprite() const
{
	// todo - factor in animations and facing
	return animation->getSprite();
	//return sprite;
}

void Player::update(Uint64 deltaMillis)
{
	animation->update(deltaMillis);
}
