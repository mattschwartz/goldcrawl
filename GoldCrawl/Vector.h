#pragma once

struct Vector
{
	float x, y;

	Vector operator+(const Vector& other) const
	{
		return {
			x + other.x,
			y + other.y
		};
	}

	Vector operator*(const Vector& other) const
	{
		return {
			x * other.x,
			y * other.y
		};
	}

	Vector operator-(const Vector& other) const
	{
		return {
			x - other.x,
			y - other.y
		};
	}
};
