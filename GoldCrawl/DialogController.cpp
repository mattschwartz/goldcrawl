#include "DialogController.h"
#include "Color.h"
#include "GameManager.h"

// every x ms, reveal a new character
constexpr auto CHAR_REVEAL_DELAY_MILLIS = 33;

void DialogController::startDialog(const std::string& dialogText)
{
	this->dialogText = dialogText;
	pages.clear();
	renderDialogLines.clear();
	numCharsRevealed = 0;
	nextCharRevealTimer = SDL_GetTicks64() + CHAR_REVEAL_DELAY_MILLIS; // reveal in now + X ms
	dialogPageFinished = false;

	pageNumber = 0;
	int i = 0;
	pages.push_back("");
	// split ';' into different pages of dialog
	// for being hacky
	for (auto c : dialogText)
	{
		if (c == ';')
		{
			++i;
			pages.push_back("");
			continue;
		}
		pages[i] += c;
	}
}

void DialogController::handleInput(const Input& input)
{
	if (input.isBindingPressed(KeyBinding::A) || input.isBindingPressed(KeyBinding::B))
	{
		if (dialogPageFinished)
		{
			++pageNumber;
			if (pageNumber >= pages.size())
			{
				GameManager::only().closeDialog();
			}
			else
			{
				dialogPageFinished = false;
				numCharsRevealed = 0;
				nextCharRevealTimer = SDL_GetTicks64() + CHAR_REVEAL_DELAY_MILLIS;
				updateRevealedChars();
			}
			return;
		}

		// not all the characters have been revealed yet,
		// so reveal them all because the player is in a hurry
		if (numCharsRevealed < pages[pageNumber].size())
		{
			numCharsRevealed = pages[pageNumber].size() - 1;
			updateRevealedChars();
		}
	}
}

void DialogController::update(Uint64 delta)
{
	if (dialogPageFinished) return;

	if (nextCharRevealTimer <= SDL_GetTicks64())
	{
		++numCharsRevealed;
		if (numCharsRevealed >= pages[pageNumber].size())
		{
			numCharsRevealed = pages[pageNumber].size() - 1;
		}
		nextCharRevealTimer = SDL_GetTicks64() + CHAR_REVEAL_DELAY_MILLIS;
		updateRevealedChars();
	}

	if (numCharsRevealed >= pages[pageNumber].size() - 1)
	{
		dialogPageFinished = true;
	}
}

void DialogController::render(const Renderer& renderer) const
{
	renderer.drawBox({ 0, 0, SCREEN_WIDTH, 66 }, colors::highlight, false);
	renderer.drawBox({ 1, 1, SCREEN_WIDTH - 2, 64 }, colors::darkest, true);

	int y = 2;
	for (auto& str : renderDialogLines)
	{
		renderer.drawText(str, 2, y, colors::highlight);
		y += 16; // line height;
	}
}

void DialogController::updateRevealedChars()
{
	int line = 0;
	renderDialogLines.clear();
	for (int i = 0; i <= numCharsRevealed / 22; ++i)
	{
		renderDialogLines.push_back("");
	}
	for (int i = 0; i <= numCharsRevealed; ++i)
	{
		renderDialogLines[i / 22] += pages[pageNumber][i];
	}
}
