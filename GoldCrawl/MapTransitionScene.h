#pragma once

#include "Prelude.h"
#include "Renderer.h"

enum class FadeDirection
{
	In,
	Out
};

class MapTransitionScene
{
public:
	void startFadeIn();
	void startFadeOut();

	bool isComplete() const;
	void update(Uint64 deltaMillis);
	void render(const Renderer& renderer) const;

private:
	float opacity;
	FadeDirection fade;
	Uint64 remainingDurationMillis;
};
