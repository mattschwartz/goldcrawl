#pragma once

#include "PlayerController.h"
#include "Vector.h"
#include "Map.h"

class WorldPlayerController : public PlayerController
{
public:
	WorldPlayerController(std::unique_ptr<Player> player, std::unique_ptr<Map> currentMap);

	Map* getMap() const { return currentMap.get(); }
	Vector getMapOffset() const { return mapOffset; }

	void handleInput(const Input& input) override;
	void update(Uint64 deltaMillis) override;

private:
	std::unique_ptr<Map> currentMap;
	Vector mapOffset;

	bool canMove(Vector& newPosition) const;
};
