#include "Map.h"

Map::Map() : totalDirtLevel(0)
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
	totalDirtLevel += tile->maxHealth;
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

void Map::addPortal(std::shared_ptr<Portal> portal)
{
	portals.push_back(std::move(portal));
}

Portal* Map::getPortal(Vector position)
{
	const auto isCollision = [](float x, float y, SDL_Rect bounds) -> bool {
		return x < bounds.x + bounds.w &&
			x + TILE_SIZE > bounds.x &&
			y < bounds.y + bounds.h &&
			y + TILE_SIZE > bounds.y;
		};

	for (auto& portal : portals)
	{
		if (isCollision(position.x, position.y, portal->bounds))
		{
			return portal.get();
		}
	}
	return nullptr;
}

void Map::addPointOfInterest(std::shared_ptr<POI> poi)
{
	pointsOfInterest.push_back(std::move(poi));
}

POI* Map::getPointOfInterest(const std::string& poiName)
{
	for (auto& poi : pointsOfInterest)
	{
		if (poi->name == poiName)
		{
			return poi.get();
		}
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
			interactableTiles.erase(t);
			tiles.erase(t);
		}
	}
}
