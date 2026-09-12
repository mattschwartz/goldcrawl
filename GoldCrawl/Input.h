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
	Input();
	~Input();

	InputUpdateResults update();
	bool isBindingDown(KeyBinding binding) const;
	bool isBindingPressed(KeyBinding binding) const;

private:
	int keyCount;
	const Uint8* keyboardState; // tbd - can't remember why i originally needed this
	Uint8* previousKeyboardState;
	Uint8* currentKeyboardState;
	
	SDL_GameController* gamepad;
	std::unordered_map<KeyBinding, bool> previousGamepadState;
	std::unordered_map<KeyBinding, bool> currentGamepadState;

	std::unordered_map<KeyBinding, std::vector<SDL_Scancode>> keyboardBindings;
	std::unordered_map<KeyBinding, std::vector<SDL_GameControllerButton>> gamepadBindings;

	void initKeybindings();
};

