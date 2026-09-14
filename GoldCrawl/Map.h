#pragma once

#include "Prelude.h"
#include "Tile.h"
#include "Vector.h"
#include "Point.h"

class Map
{
public:
	using Tilemap = std::unordered_map<Point, std::shared_ptr<Tile>>;

	/// <summary>
	/// returns all the tiles at the specified position, with topmost tile first,
	/// depending on sortedTileLayer
	/// </summary>
	std::vector<std::shared_ptr<Tile>> getTilesAt(Vector position) const;
	Tilemap getTiles(TileLayer layer) const;

private:
	// tiles
	std::unordered_map<TileLayer, Tilemap> tilesByLayer;
};
