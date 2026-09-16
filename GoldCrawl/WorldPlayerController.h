#pragma once

#include "PlayerController.h"
#include "Vector.h"
#include "Map.h"

constexpr auto TRANSITION_DURATION = 1000;

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
	Vector transitioningToMapOffset;
	bool sceneTransitioning;
	long transitionDurationMillis;

	bool canMove(Vector& newPosition) const;
};
