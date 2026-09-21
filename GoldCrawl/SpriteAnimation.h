#pragma once

#include <string>
#include <memory>
#include <SDL.h>
#include <unordered_map>
#include <vector>
#include "Sprite.h"

struct SpriteFrame
{
	SDL_Rect src;
	int duration;
};

class SpriteAnimation
{
public:
	SpriteAnimation(const std::string& filepath, const std::string& startingCycle, bool loop);

	const Sprite& getSprite() const { return *currentSprite; }
	void setCycle(const std::string& cycle);
	bool isComplete() const { return complete; }
	void stop() { complete = true; }

	// update the animation
	void update(Uint64 deltaMillis);

private:
	std::unique_ptr<Sprite> currentSprite;
	std::string currentCycle;
	SpriteFrame* currentFrame;
	int frameIndex;
	int hold;
	bool loop;
	bool complete;
	std::vector<std::unique_ptr<SpriteFrame>> frames;
	std::unordered_map<std::string, std::vector<SpriteFrame*>> cycles;
};
