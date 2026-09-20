#pragma once

#include "PlayerController.h"
#include "Vector.h"
#include "Map.h"

constexpr auto TRANSITION_DURATION = 1000;

class WorldPlayerController : public PlayerController
{
public:
	bool isLoadingNewMap;

	WorldPlayerController(std::unique_ptr<Player> player, std::shared_ptr<Map> currentMap);

	bool shouldShowDirtMarkers() const { return showDirtMarkers; }
	void switchToLoadingMap();

	Tile* getTargetedTile() const;
	Map* getMap() const { return currentMap.get(); }
	Vector getMapOffset() const { return mapOffset; }

	void handleInput(const Input& input) override;
	void update(Uint64 deltaMillis) override;

private:
	std::weak_ptr<Tile> targetedTile;
	std::shared_ptr<Map> currentMap;
	Vector mapOffset;
	Vector transitioningToMapOffset;
	bool sceneTransitioning;
	long transitionDurationMillis;
	bool showDirtMarkers;

	Vector newPlayerPosition;
	std::shared_ptr<Map> loadingMap;

	bool canMove(Vector& newPosition) const;
	bool enterPortal();
	// based on where the player is and is facing, is there anything to interact with
	bool testInteractables();
};
