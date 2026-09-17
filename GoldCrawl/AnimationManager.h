#pragma once

#include <vector>
#include "Renderer.h"
#include "SpriteAnimation.h"

struct Animation
{
	std::shared_ptr<SpriteAnimation> animation;
	SDL_Rect bounds;
};

class AnimationManager
{
public:
	static AnimationManager& only();

	void addOneShot(const std::string& filepath, SDL_Rect bounds);

	void update(Uint64 delta);
	void render(const Renderer& renderer) const;

private:
	std::vector<std::shared_ptr<Animation>> animations;

	AnimationManager() = default;
	
};
