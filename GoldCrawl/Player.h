#pragma once

#include "Prelude.h"
#include "Vector.h"
#include "Sprite.h"
#include "SpriteAnimation.h"
#include "Renderer.h"
#include "Broom.h"

class Player
{
public:
	Player();

	const Sprite& getSprite() const;
	Broom* getBroom() const { return broom.get(); }

	void update(Uint64 deltaMillis);

	Vector getDirection() const { return direction; }
	void setDirection(Vector direction);

	Vector getFacing() const { return facing; }

	/// <summary>
	/// position within a map, so needs to be offset for drawing
	/// </summary>
	Vector getPosition() const { return position; }
	void setPosition(Vector position) { this->position = position; }

private:
	Vector position;
	Vector direction;
	// doesn't change when player isn't moving, unlike
	// direction, which resets when not moving
	Vector facing;
	Sprite sprite;
	std::unique_ptr<Broom> broom;
	std::unique_ptr<SpriteAnimation> animation;
};
