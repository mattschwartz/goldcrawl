#pragma once

#include "Prelude.h"
#include "Vector.h"
#include "Map.h"
#include "SpriteAnimation.h"

class Broom
{
public:
	Broom();

	/// <summary>
	/// sweep the area around the center, cleaning debris
	/// </summary>
	/// <param name="center"></param>
	void sweepBroom(Vector center, Map* map);
	void update(Uint64 deltaMillis);

	SDL_Rect getPosition() const { return renderPosition; }
	const Sprite& getSprite() const;
	bool isAnimating() const { return !sweepAnimation->isComplete(); }

private:
	// for rendering when visible
	SDL_Rect renderPosition;
	std::unique_ptr<SpriteAnimation> sweepAnimation;
};
