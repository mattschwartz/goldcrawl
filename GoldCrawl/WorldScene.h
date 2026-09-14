#pragma once

#include "Prelude.h"
#include "Scene.h"
#include "WorldPlayerController.h"
#include "Map.h"

class WorldScene : public Scene
{
public:
	WorldScene(std::unique_ptr<Map> currentMap);

	void handleInput(const Input& input) override;
	void update(Uint64 delta) override;
	void render(const Renderer& renderer) const override;

private:
	std::unique_ptr<WorldPlayerController> controller;

	void renderMap(const Renderer& renderer) const;
	void renderPlayer(const Renderer& renderer) const;
};
