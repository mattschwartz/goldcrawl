#pragma once

#include "Input.h"
#include "Renderer.h"

class Director
{
public:
	void update();
	void handleInput(Input& input);
	void render(const Renderer& renderer);
};

