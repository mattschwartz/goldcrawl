#pragma once

#include "Prelude.h"
#include "Scene.h"
#include "WorldPlayerController.h"
#include "MapTransitionScene.h"
#include "Map.h"

class WorldScene : public Scene
{
public:
	WorldScene();

	void handleInput(const Input& input) override;
	void update(Uint64 delta) override;
	void render(const Renderer& renderer) const override;

private:
	bool isFadingOutScene;
	bool isFadingInScene;
	std::unique_ptr<WorldPlayerController> controller;
	std::unique_ptr<MapTransitionScene> transitionScene;

	void renderMap(const Renderer& renderer) const;
	void renderPlayer(const Renderer& renderer) const;
	void renderToolbar(const Renderer& renderer) const;
};
