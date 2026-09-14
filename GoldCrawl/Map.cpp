#include "Map.h"

Map::Map()
{
	for (int x = 0; x < 10; ++x)
	{
		for (int y = 0; y < 9; ++y)
		{
			tilesByLayer[TileLayer::Ground].emplace(Point{ x,y }, 
				std::make_shared<Tile>("Sprites/sand.png"));
		}
	}
}

std::vector<std::shared_ptr<Tile>> Map::getTilesAt(Vector position) const
{
	Point p{ (int)position.x, (int)position.y };
	std::vector<std::shared_ptr<Tile>> result;
	for (auto& layer : sortedTileLayers)
	{
		if (auto it = tilesByLayer.find(layer); it != tilesByLayer.end())
		{
			if (auto tile = it->second.find(p); tile != it->second.end())
			{
				result.push_back(tile->second);
			}
		}
	}
	return result;
}

Map::Tilemap Map::getTiles(TileLayer layer) const
{
	std::vector<std::shared_ptr<Tile>> result;
	if (auto it = tilesByLayer.find(layer); it != tilesByLayer.end())
	{
		return it->second;
	}
	return {};
}
