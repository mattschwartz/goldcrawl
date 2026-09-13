#pragma once

#include "Prelude.h"
#include "Scene.h"
#include "WorldPlayerController.h"

class WorldScene : public Scene
{
public:
	WorldScene();

	void handleInput(const Input& input) override;
	void update(Uint64 delta) override;
	void render(const Renderer& renderer) const override;

private:
	std::unique_ptr<WorldPlayerController> controller;
};
