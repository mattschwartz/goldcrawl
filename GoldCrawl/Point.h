#pragma once

struct Point
{
	int x, y;

	Point operator+(const Point& other) const
	{
		return {
			x + other.x,
			y + other.y
		};
	}

	Point operator*(const Point& other) const
	{
		return {
			x * other.x,
			y * other.y
		};
	}

	Point operator-(const Point& other) const
	{
		return {
			x - other.x,
			y - other.y
		};
	}

	bool operator==(const Point& other) const
	{
		return other.x == x && other.y == y;
	}
};

/// <summary>
/// https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
/// </summary>
template<>
struct std::hash<Point>
{
	std::size_t operator()(const Point& p) const
	{
		return (std::hash<int>()(p.x) << 1)
			^ std::hash<int>()(p.y);
	}
};