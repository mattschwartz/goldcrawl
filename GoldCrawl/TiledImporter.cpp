#include "TiledImporter.h"

#include <nlohmann/json.hpp>
#include "FileSystem.h"
#include "TrapTile.h"
#include "LootTile.h"
#include "Portal.h"
#include "POI.h"

using namespace nlohmann;

std::unique_ptr<Map> TiledImporter::parseTiledMap(const std::string& filepath)
{
	json j = json::parse(bacon::fs::readText(filepath).value_or(""));
	if (!j.contains("tilesets") || !j["tilesets"].is_array()) throw tiled::TiledError("'tilesets' missing or not an array");
	if (!j.contains("layers") || !j["layers"].is_array()) throw tiled::TiledError("'layers' missing or not an array");

	// todo - doesn't support more than 1 tileset atm
	std::string tilesetSrc = j["tilesets"][0]["source"];
	SDL_Log("Loading tileset %s", tilesetSrc.c_str());
	auto tileset = parseTileset("Maps/" + tilesetSrc); // todo - cheating with the "Maps/" part, but oh well, relative filepaths can wait
	tileset->firstGid = j["tilesets"][0]["firstgid"];

	auto map = std::make_unique<Map>();

	for (auto& jLayer : j["layers"])
	{
		if (jLayer["type"] == "objectgroup")
		{
			parseObjectLayer(jLayer, *map);
		}
		else if (jLayer["type"] == "tilelayer")
		{
			parseTileLayer(jLayer, *map, *tileset);
		}
	}

	return map;
}

void TiledImporter::parseTileLayer(const nlohmann::json& jLayer, Map& map, tiled::Tileset& tileset)
{
	std::string layerName = jLayer["name"];
	if (TileLayersByName.find(layerName) == TileLayersByName.end())
	{
		SDL_LogError(0, "Layer name %s doesn't exist", layerName.c_str());
		return;
	}
	if (!jLayer.contains("chunks") || !jLayer["chunks"].is_array()) throw tiled::TiledError("'chunks' is missing or not an array");

	TileLayer layer = TileLayersByName[layerName];

	for (auto& jChunk : jLayer["chunks"])
	{
		tiled::TilemapLayerChunk chunk = jChunk;
		int i = 0;
		for (int y = 0; y < tiled::TILES_HEIGHT; ++y)
		{
			for (int x = 0; x < tiled::TILES_WIDTH; ++x)
			{
				int tileX = x + chunk.x;
				int tileY = y + chunk.y;
				int tileId = chunk.data[i++];
				if (tileId == 0) continue;
				auto tt = tileset.getTile(tileId);
				if (!tt) throw tiled::TiledError("no such tile for id " + tileId);

				std::shared_ptr<Tile> tile;

				// todo - construct the specific type of tile
				auto interaction = tt->getInteraction();
				if (interaction == "reset_trap")
				{
					std::string spriteName = tt->getStringProp("trap_set_sprite").value_or("");
					tile = std::make_shared<TrapTile>("Sprites/" + spriteName);
				}
				else if (interaction == "loot_tile")
				{
					tile = std::make_shared<LootTile>();
				}
				else if (interaction == "clean")
				{
					tile = std::make_shared<Tile>();
					tile->cleanable = true;
				}
				else // default tile
				{
					tile = std::make_shared<Tile>();
				}

				if (tt->getAnimation().has_value())
				{
					std::string animationFilepath = "Sprites/" + tt->getAnimation().value();
					auto tileAnimation = std::make_unique<SpriteAnimation>(animationFilepath, "", true);
					tile->animation = std::move(tileAnimation);
				}
				else
				{
					tile->sprite = tt->image.substr(3);
				}

				tile->position = { tileX, tileY };
				tile->setCollision(tt->hasCollision());
				tile->maxHealth = tile->currentHealth = tt->getHealth();
				map.setTile(layer, tileX, tileY, tile);
			}
		}
	}
}

static std::optional<std::string> getProp(const nlohmann::json& j, std::string propertyName)
{
	if (!j.contains("properties") || !j["properties"].is_array()) return std::nullopt;
	for (auto& jProp : j["properties"])
	{
		if (jProp["name"] == propertyName)
		{
			if (jProp["type"] == "string")
			{
				return jProp["value"].get<std::string>();
			}
			else
			{
				return std::nullopt;
			}
		}
	}

	return std::nullopt;
}

void TiledImporter::parseObjectLayer(const nlohmann::json& j, Map& map)
{
	if (!j.contains("objects") || !j["objects"].is_array()) throw tiled::TiledError("'objects' missing from object layer");

	for (auto& jObject : j["objects"])
	{
		std::string type = getProp(jObject, "type").value_or("");
		if (type == "portal")
		{
			int x = jObject["x"];
			int y = jObject["y"];
			int w = jObject["width"];
			int h = jObject["height"];
			auto destinationMap = getProp(jObject, "destination_map");
			auto spawnPoi = getProp(jObject, "spawn_poi");

			if (destinationMap.has_value() && spawnPoi.has_value())
			{
				// create a portal tile to transport the player to a new map
				auto portal = std::make_shared<Portal>("Maps/" + *destinationMap + ".tmj", *spawnPoi);
				portal->bounds = { x, y, w, h };
				map.addPortal(portal);
			}
		}
		else
		{
			// create POI
			auto poi = std::make_shared<POI>();
			poi->name = jObject["name"];
			poi->position.x = (int)jObject["x"].get<float>();
			poi->position.y = (int)jObject["y"].get<float>();
			map.addPointOfInterest(poi);
		}
	}
}

std::unique_ptr<tiled::Tileset> TiledImporter::parseTileset(const std::string& filepath)
{
	std::unique_ptr<tiled::Tileset> result = std::make_unique<tiled::Tileset>();
	json j = json::parse(bacon::fs::readText(filepath).value_or(""));

	if (!j.contains("tiles") || !j["tiles"].is_array()) throw tiled::TiledError("'tiles' missing or not array");

	for (auto& jTile : j["tiles"])
	{
		auto tile = std::make_shared<tiled::Tile>();
		tile->id = jTile["id"];
		tile->image = jTile["image"];
		tile->imageHeight = jTile["imageheight"];
		tile->imageWidth = jTile["imagewidth"];

		if (jTile.contains("properties") && jTile["properties"].is_array())
		{
			for (auto& jProp : jTile["properties"])
			{
				auto prop = std::make_shared<tiled::TileProperty>();
				prop->name = jProp["name"];
				prop->type = jProp["type"];
				if (prop->type == "bool")
				{
					prop->value = jProp["value"].get<bool>();
				}
				if (prop->type == "float")
				{
					prop->value = jProp["value"].get<float>();
				}
				if (prop->type == "string")
				{
					prop->value = jProp["value"].get<std::string>();
				}
				tile->properties.push_back(std::move(prop));
			}
		}
		result->tiles.push_back(std::move(tile));
	}

	return result;
}

std::shared_ptr<tiled::Tile> tiled::Tileset::getTile(int id)
{
	if (id == 0) return nullptr;

	for (auto& t : tiles)
	{
		if (t->id == id - firstGid) return t;
	}
	return nullptr;
}

bool tiled::Tile::hasCollision() const
{
	for (auto& prop : properties)
	{
		if (prop->name == "hasCollision")
		{
			// https://en.cppreference.com/cpp/utility/variant/get
			return std::get<bool>(prop->value);
		}
	}
	return false;
}

float tiled::Tile::getHealth() const
{
	for (auto& prop : properties)
	{
		if (prop->name == "health")
		{
			return std::get<float>(prop->value);
		}
	}
	return 0.0;
}

std::optional<std::string> tiled::Tile::getAnimation() const
{
	return getStringProp("animation");
}

std::optional<std::string> tiled::Tile::getInteraction() const
{
	return getStringProp("interaction");
}

std::optional<std::string> tiled::Tile::getStringProp(const std::string& propName) const
{
	for (auto& prop : properties)
	{
		if (prop->name == propName)
		{
			return std::get<std::string>(prop->value);
		}
	}
	return std::nullopt;
}
