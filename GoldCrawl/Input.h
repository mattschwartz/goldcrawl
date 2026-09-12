#pragma once

#include "Prelude.h"

// gameboy key bindings available to us
enum class KeyBinding
{
	Left,
	Right,
	Up,
	Down,
	Start,
	Select,
	A,
	B
};

enum class InputUpdateResults
{
	None,
	Quit
};

class Input
{
public:
	// todo - probably should move to an init variant, but oh well
	// it's probably not going to cause issues
	static Input& only();
	~Input();

	InputUpdateResults update();
	bool isBindingDown(KeyBinding binding) const;

private:
	Input();

	int keyCount;
	const Uint8* keyboardState; // tbd - can't remember why i originally needed this
	Uint8* previousKeyboardState;
	Uint8* currentKeyboardState;
	SDL_GameController* gamepad;

	std::unordered_map<KeyBinding, std::vector<SDL_Scancode>> keyboardBindings;
	std::unordered_map<KeyBinding, std::vector<SDL_GameControllerButton>> gamepadBindings;

	void initKeybindings();
};

