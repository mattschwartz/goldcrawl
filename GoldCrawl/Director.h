#pragma once

#include "Prelude.h"
#include "Input.h"
#include "Renderer.h"
#include "Scene.h"

class Director
{
public:
	Director(std::unique_ptr<Scene> startingScene);

	void update();
	void handleInput(Input& input);
	void render(const Renderer& renderer);

private:
	Uint64 lastTicks;
	std::unique_ptr<Scene> scene;
};

