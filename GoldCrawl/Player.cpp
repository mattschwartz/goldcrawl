#include "Player.h"

Player::Player(Vector position) :
	position(position),
	facing(),
	direction(),
	sprite(Sprite{ "Sprites/charlie.png" }),
	broom(std::make_unique<Broom>()),
	animation(std::make_unique<SpriteAnimation>("Sprites/charlie.json", "idle_right", true))
{
}

const Sprite& Player::getSprite() const
{
	return animation->getSprite();
}

void Player::update(Uint64 deltaMillis)
{
	animation->update(deltaMillis);
	broom->update(deltaMillis);
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
		else if (oldDirection.x < 0)
		{
			animation->setCycle("idle_left");
		}
		else if (oldDirection.y < 0)
		{
			animation->setCycle("idle_up");
		}
		else
		{
			animation->setCycle("idle_down");
		}
		return;
	}
	if (direction.x > 0)
	{
		facing = { 1,0 };
		animation->setCycle("walk_right");
		return;
	}
	if (direction.x < 0)
	{
		facing = { -1,0 };
		animation->setCycle("walk_left");
		return;
	}
	if ( direction.y > 0)
	{
		facing = { 0, 1 };
		animation->setCycle("walk_down");
		return;
	}
	if (direction.y < 0)
	{
		facing = { 0, -1 };
		animation->setCycle("walk_up");
		return;
	}
}
