#include "Player.h"

Player::Player() :
	position({ 16, 16 }),
	direction(),
	sprite(Sprite{ "Sprites/charlie.png" }),
	animation(std::make_unique<SpriteAnimation>("Sprites/charlie.json", "idle_right"))
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

void Player::setDirection(Vector direction)
{
	if (direction == this->direction) return;
	Vector oldDirection = this->direction;
	this->direction = direction;

	if (direction == Vector{ 0,0 })
	{
		if (oldDirection.x > 0)
		{
			animation->setCycle("idle_right");
		}
		else
		{
			animation->setCycle("idle_left");
		}
		return;
	}
	if (oldDirection.x <= 0 && direction.x > 0)
	{
		animation->setCycle("walk_right");
		return;
	}
	if (oldDirection.x >= 0 && direction.x < 0)
	{
		animation->setCycle("walk_left");
		return;
	}
	if (oldDirection.y <= 0 && direction.y > 0)
	{
		animation->setCycle("walk_down");
		return;
	}
	if (oldDirection.y >= 0 && direction.y < 0)
	{
		animation->setCycle("walk_up");
		return;
	}
}
