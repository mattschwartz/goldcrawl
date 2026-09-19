#pragma once

#include "Tile.h"

class LootTile : public Tile
{
public:
	bool isInteractable() const override { return true; }
	void interact() override;

private:

};
