#pragma once

#include "Prelude.h"
#include "Vector.h"
#include "Sprite.h"
#include "SpriteAnimation.h"

class Player
{
public:
	Player();

	const Sprite& getSprite() const;

	void update(Uint64 deltaMillis);

	Vector getDirection() const { return direction; }
	void setDirection(Vector direction);

	/// <summary>
	/// position within a map, so needs to be offset for drawing
	/// </summary>
	Vector getPosition() const { return position; }
	void setPosition(Vector position) { this->position = position; }

private:
	Vector position;
	Vector direction;
	Sprite sprite;
	std::unique_ptr<SpriteAnimation> animation;
};
