#pragma once

#include "Prelude.h"
#include "Tile.h"
#include "Vector.h"
#include "Point.h"
#include "Portal.h"
#include "POI.h"

class Map
{
public:
	using Tilemap = std::unordered_map<Point, std::shared_ptr<Tile>>;

	Map();

	bool didFinishCleaning() const { return totalDirtLevel == 0 || finishedCleaning; }
	void setFinishedCleaning(bool finishedCleaning) { this->finishedCleaning = finishedCleaning; }

	/// <summary>
	/// returns all the tiles at the specified position, with topmost tile first,
	/// depending on sortedTileLayer
	/// </summary>
	std::vector<std::shared_ptr<Tile>> getTilesAt(int x, int y) const;
	Tilemap getTiles(TileLayer layer) const;

	float getTotalDirtLevel() const { return totalDirtLevel; }
	float getCleanDamage() const { return cleanLevel; }
	void addCleanDamage(float amt) { cleanLevel += amt; }

	void setTile(TileLayer layer, int x, int y, std::shared_ptr<Tile> tile);
	bool hasCollision(int x, int y) const;
	std::shared_ptr<Tile> getInteractable(int x, int y) const;

	void addPortal(std::shared_ptr<Portal> portal);
	Portal* getPortal(Vector position);

	void addPointOfInterest(std::shared_ptr<POI> poi);
	POI* getPointOfInterest(const std::string& poiName);

	void update(Uint64 deltaMillis);

private:
	bool finishedCleaning;
	// how clean the dungeon is
	// cleanLevel / totalDirtLevel = cleanliness %
	float cleanLevel;
	// just how dirty this map really is
	// sum of all dirty tiles
	float totalDirtLevel;
	// tiles
	std::unordered_map<TileLayer, Tilemap> tilesByLayer;
	Tilemap collisionTiles;
	Tilemap interactableTiles;
	std::vector<std::shared_ptr<Portal>> portals;
	std::vector<std::shared_ptr<POI>> pointsOfInterest;
};
