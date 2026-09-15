#pragma once

#include <vector>
#include <memory>
#include <string>
#include <variant>
#include <nlohmann/json.hpp>
#include "Map.h"

namespace tiled
{
	constexpr auto TILES_WIDTH = 10;
	constexpr auto TILES_HEIGHT = 8;

	struct TiledError : public std::exception
	{
		TiledError(const std::string& msg) : std::exception(msg.c_str()) {}
	};

	struct TileProperty
	{
		std::string name;
		std::string type;
		std::variant<bool, float, std::string> value;
	};

	struct Tile
	{
		int id;
		std::string image;
		int imageHeight;
		int imageWidth;
		std::vector<std::shared_ptr<TileProperty>> properties;
	};

	struct Tileset
	{
		int firstGid;
		std::vector<std::shared_ptr<Tile>> tiles;
		std::shared_ptr<Tile> getTile(int id);
	};

	struct TilemapLayerChunk
	{
		// each chunk has 10 columns of 8 rows for 80 cells worth of data
		// which tiled store's as numbers corresponding to nothing (0) or
		// a tile id
		int data[80];
		int x, y;
	};

	struct TilemapLayer
	{
		std::string name;
		int startX, startY;
	};

	struct Tilemap
	{

	};

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TilemapLayerChunk, data, x, y)
}

class TiledImporter
{
public:
	std::unique_ptr<Map> parseTiledMap(const std::string& filepath);

private:
	std::unique_ptr<tiled::Tileset> parseTileset(const std::string& filepath);
};
