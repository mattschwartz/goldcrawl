#pragma once

#include "Renderer.h"

class Director
{
public:
	void update();
	void handleInput();
	void render(const Renderer& renderer);
};

