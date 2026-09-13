#pragma once

#include "Prelude.h"
#include "Renderer.h"
#include "Input.h"

class Scene
{
public:
	virtual void handleInput(const Input& input) {}
	virtual void update(Uint64 delta) {}
	virtual void render(const Renderer& renderer) const {}
};
