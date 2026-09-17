#include "Map.h"

Map::Map()
{
	for (auto& layer : SortedTileLayers)
	{
		tilesByLayer.emplace(layer, std::unordered_map<Point, std::shared_ptr<Tile>>{});
	}
}

std::vector<std::shared_ptr<Tile>> Map::getTilesAt(int x, int y) const
{
	Point p{ x, y };
	std::vector<std::shared_ptr<Tile>> result;
	for (auto& layer : SortedTileLayers)
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

void Map::setTile(TileLayer layer, int x, int y, std::shared_ptr<Tile> tile)
{
	if (tile->hasCollision())
	{
		collisionTiles.emplace(Point{ x,y }, tile);
	}
	if (tile->isInteractable())
	{
		interactableTiles.emplace(Point{ x,y }, tile);
	}
	tilesByLayer[layer].emplace(Point{ x,y }, std::move(tile));
}

bool Map::hasCollision(int x, int y) const
{
	return collisionTiles.find(Point{ x,y }) != collisionTiles.end();
}

std::shared_ptr<Tile> Map::getInteractable(int x, int y) const
{
	if (auto it = interactableTiles.find(Point{ x,y }); it != interactableTiles.end())
	{
		if (it->second->isInteractable()) return it->second;
		else return nullptr;
	}
	return nullptr;
}

void Map::update(Uint64 deltaMillis)
{
	for (auto& [_, tiles] : tilesByLayer)
	{
		std::vector<Point> toRemove;
		for (auto& [point, tile] : tiles)
		{
			tile->update(deltaMillis);

			if (tile->shouldRemove)
			{
				toRemove.push_back(point);
			}
		}
		for (auto& t : toRemove)
		{
			tiles.erase(t);
		}
	}
}
