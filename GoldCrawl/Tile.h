#pragma once

#include "Prelude.h"

enum class TileLayer
{
	Ground,
	Doodad,
	Entities,
	Player,
	Sky
};

// the correct rendering layer for tiles
static std::vector<TileLayer> sortedTileLayers{
	TileLayer::Ground,
	TileLayer::Doodad,
	TileLayer::Entities,
	TileLayer::Player,
	TileLayer::Sky
};

class Tile
{
public:
	Tile(const std::string& sprite);

	std::string getSprite() const;
	void update(float delta); // todo - like animations

private:
	std::string sprite;
};
