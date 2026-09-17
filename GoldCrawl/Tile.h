#pragma once

#include "Prelude.h"
#include "SpriteAnimation.h"
#include "Point.h"

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
	Tile();

	virtual bool isInteractable() const { return false; }

	bool hasCollision() const { return collision; }
	void setCollision(bool collision) { this->collision = collision; }

	float getCurrentHealth() const { return currentHealth; }
	float getMaxHealth() const { return maxHealth; }
	void takeDamage(float cleanDamage);

	const Sprite& getSprite() const;

	virtual void interact() {} // player interacts with this tile
	virtual void clean() {} // broom hits this tile
	virtual void update(Uint64 delta); // todo - like animations

	friend class TiledImporter;
	friend class Map;

protected:
	std::optional<Sprite> sprite;
	std::unique_ptr<SpriteAnimation> animation;
	Point position; // ugh

private:
	bool shouldRemove;
	bool collision;
	// represents how dirty the tile is. 0.0 = clean, broom deals clean damage aginst debris
	float maxHealth;
	float currentHealth;
};
