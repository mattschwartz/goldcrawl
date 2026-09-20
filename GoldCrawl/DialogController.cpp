#include "DialogController.h"
#include "Color.h"
#include "GameManager.h"

// every x ms, reveal a new character
constexpr auto CHAR_REVEAL_DELAY_MILLIS = 33;

void DialogController::startDialog(const std::string& dialogText)
{
	this->dialogText = dialogText;
	dialogLines.clear();
	numCharsRevealed = 0;
	nextCharRevealTimer = SDL_GetTicks64() + CHAR_REVEAL_DELAY_MILLIS; // reveal in now + X ms
	dialogFinished = false;
}

void DialogController::handleInput(const Input& input)
{
	if (input.isBindingPressed(KeyBinding::A) || input.isBindingPressed(KeyBinding::B))
	{
		if (dialogFinished)
		{
			GameManager::only().closeDialog();
			return;
		}

		// not all the characters have been revealed yet,
		// so reveal them all because the player is in a hurry
		if (numCharsRevealed < this->dialogText.size())
		{
			numCharsRevealed = this->dialogText.size() - 1;
			updateRevealedChars();
		}
	}
}

void DialogController::update(Uint64 delta)
{
	if (dialogFinished) return;

	if (nextCharRevealTimer <= SDL_GetTicks64())
	{
		++numCharsRevealed;
		if (numCharsRevealed >= dialogText.size())
		{
			numCharsRevealed = dialogText.size() - 1;
		}
		nextCharRevealTimer = SDL_GetTicks64() + CHAR_REVEAL_DELAY_MILLIS;
		updateRevealedChars();
	}

	if (numCharsRevealed >= dialogText.size() - 1)
	{
		dialogFinished = true;
	}
}

void DialogController::render(const Renderer& renderer) const
{
	renderer.drawBox({ 0, 0, SCREEN_WIDTH, 60 }, colors::darkest, true);

	int y = 0;
	for (auto& str : dialogLines)
	{
		renderer.drawText(str, 0, y, colors::highlight);
		y += 16; // line height;
	}
}

void DialogController::updateRevealedChars()
{
	int line = 0;
	dialogLines.clear();
	for (int i = 0; i <= numCharsRevealed / 23; ++i)
	{
		dialogLines.push_back("");
	}
	for (int i = 0; i <= numCharsRevealed; ++i)
	{
		dialogLines[i / 23] += dialogText[i];
	}
}
