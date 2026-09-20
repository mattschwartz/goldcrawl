#pragma once

#include "Prelude.h"
#include "Input.h"
#include "Renderer.h"

class DialogController
{
public:
	void startDialog(const std::string& dialogText);
	void handleInput(const Input& input);
	void update(Uint64 delta);
	void render(const Renderer& renderer) const;

private:
	bool dialogFinished;
	std::string dialogText;
	std::vector<std::string> dialogLines;
	int numCharsRevealed;
	Uint64 nextCharRevealTimer;

	void updateRevealedChars();
};
