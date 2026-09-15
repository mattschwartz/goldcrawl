#pragma once

#include "Prelude.h"

enum class TileLayer
{
	Ground,
	// the tile is cleanable and gets hit by broom sweeps
	Cleanable,
	// the tile is interactable and can be interacted with by the player
	// if nearby
	Interactable,
	// like NPCs
	Entities,
	Player,
	Sky
};

static std::unordered_map<std::string, TileLayer> TileLayersByName{
	{"ground", TileLayer::Ground},
	{"cleanable", TileLayer::Cleanable},
	{"interactable", TileLayer::Interactable},
	{"entities", TileLayer::Entities},
	{"player", TileLayer::Player},
	{"sky", TileLayer::Sky},
};

// the correct rendering layer for tiles
static std::vector<TileLayer> SortedTileLayers{
	TileLayer::Ground,
	TileLayer::Cleanable,
	TileLayer::Interactable,
	TileLayer::Entities,
	TileLayer::Player,
	TileLayer::Sky
};

class Tile
{
public:
	Tile(const std::string& sprite);

	bool hasCollision() const { return collision; }
	void setCollision(bool collision) { this->collision = collision; }

	std::string getSprite() const;

	virtual void interact() {} // player interacts with this tile
	virtual void clean() {} // broom hits this tile
	virtual void update(Uint64 delta); // todo - like animations

private:
	std::string sprite;
	bool collision;
};
