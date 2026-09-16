#pragma once

#include "Prelude.h"
#include "Vector.h"

class Player
{
public:
	Vector getDirection() const { return direction; }
	void setDirection(Vector direction) { this->direction = direction; }

	/// <summary>
	/// position within a map, so needs to be offset for drawing
	/// </summary>
	Vector getPosition() const { return position; }
	void setPosition(Vector position) { this->position = position; }

private:
	Vector position{ 16,16 };
	Vector direction;
};
