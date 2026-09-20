#pragma once

#include "Tile.h"

class DialogTile : public Tile
{
public:
	DialogTile(const std::string& dialogText) : dialogText(dialogText)
	{
	}

	bool isInteractable() const override { return true; }
	void interact(class Player&) override;

private:
	std::string dialogText;
};
