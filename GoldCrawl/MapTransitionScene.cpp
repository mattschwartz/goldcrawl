#include "MapTransitionScene.h"
#include "Color.h"

constexpr int DURATION_MILLIS = 333;

void MapTransitionScene::startFadeIn()
{
	opacity = 1;
	remainingDurationMillis = DURATION_MILLIS;
	fade = FadeDirection::In;
}

void MapTransitionScene::startFadeOut()
{
	opacity = 0;
	remainingDurationMillis = DURATION_MILLIS;
	fade = FadeDirection::Out;
}

bool MapTransitionScene::isComplete() const
{
	return remainingDurationMillis <= 0;
}

void MapTransitionScene::update(Uint64 deltaMillis)
{
	if (remainingDurationMillis <= 0) return;

	remainingDurationMillis -= deltaMillis;
	remainingDurationMillis = std::max((int)remainingDurationMillis, 0);

	if (fade == FadeDirection::In)
	{
		opacity = (float)remainingDurationMillis / DURATION_MILLIS;
	}
	else if (fade == FadeDirection::Out)
	{
		opacity = 1 - (float)remainingDurationMillis / DURATION_MILLIS;
	}
}

void MapTransitionScene::render(const Renderer& renderer) const
{
	if (remainingDurationMillis <= 0) return;

	auto a = (Uint8)(opacity * 255);
	SDL_Color color{
		colors::highlight.r,
		colors::highlight.g,
		colors::highlight.b,
		a
	};
	renderer.drawBox({ 0,0,SCREEN_WIDTH,SCREEN_HEIGHT }, color, true);
}
