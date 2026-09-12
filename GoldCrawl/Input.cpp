#include "Input.h"

Input::Input()
{
    initKeybindings();

    keyboardState = SDL_GetKeyboardState(&keyCount);
    previousKeyboardState = new Uint8[keyCount];
    currentKeyboardState = new Uint8[keyCount];

    // we've got a gamepad
    if (SDL_NumJoysticks() > 0)
    {
        gamepad = SDL_GameControllerOpen(0);
        SDL_Log("gamepad connected");
    }
    else
    {
        gamepad = nullptr;
        SDL_Log("No gamepad found");
    }
}

Input::~Input()
{
    delete[] previousKeyboardState;
    delete[] currentKeyboardState;
    if (gamepad)
    {
        SDL_GameControllerClose(gamepad);
    }

    previousKeyboardState = nullptr;
    currentKeyboardState = nullptr;
    gamepad = nullptr;
}

InputUpdateResults Input::update()
{
    memcpy(previousKeyboardState, currentKeyboardState, keyCount);
    SDL_PumpEvents();
    memcpy(currentKeyboardState, keyboardState, keyCount);

    // gamepad connected mid-game
    if (!gamepad && SDL_NumJoysticks() > 0)
    {
        gamepad = SDL_GameControllerOpen(0);
        SDL_Log("gamepad connected");
    }

    if (gamepad)
    {
        // process gamepad bindings to tell pressed vs down
        for (auto& [binding, buttons] : gamepadBindings)
        {
            previousGamepadState[binding] = currentGamepadState[binding];
            bool isDown = false;
            for (auto& btn : buttons)
            {
                if (SDL_GameControllerGetButton(gamepad, btn))
                {
                    isDown = true;
                }
            }
            currentGamepadState[binding] = isDown;
        }
    }

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            return InputUpdateResults::Quit;
        }
    }

    return InputUpdateResults::None;
}

bool Input::isBindingDown(KeyBinding binding) const
{
    if (auto it = keyboardBindings.find(binding); it != keyboardBindings.end())
    {
        for (auto& scanCode : it->second)
        {
            if (currentKeyboardState[scanCode]) return true;
        }
    }

    if (gamepad)
    {
        if (auto it = gamepadBindings.find(binding); it != gamepadBindings.end())
        {
            for (auto& button : it->second)
            {
                if (SDL_GameControllerGetButton(gamepad, button)) return true;
            }
        }
    }

    return false;
}

bool Input::isBindingPressed(KeyBinding binding) const
{
    if (auto it = keyboardBindings.find(binding); it != keyboardBindings.end())
    {
        for (auto& scanCode : it->second)
        {
            if (currentKeyboardState[scanCode] && !previousKeyboardState[scanCode]) return true;
        }
    }

    if (gamepad)
    {
        auto prev = previousGamepadState.find(binding);
        auto cur = currentGamepadState.find(binding);
        if (prev != previousGamepadState.end() && cur != currentGamepadState.end() && !prev->second && cur->second)
        {
            return true;
        }
    }

    return false;
}

void Input::initKeybindings()
{
    // keyboard controls

    keyboardBindings[KeyBinding::Up] = { SDL_SCANCODE_W, SDL_SCANCODE_UP };
    keyboardBindings[KeyBinding::Down] = { SDL_SCANCODE_S, SDL_SCANCODE_DOWN };
    keyboardBindings[KeyBinding::Left] = { SDL_SCANCODE_A, SDL_SCANCODE_LEFT };
    keyboardBindings[KeyBinding::Right] = { SDL_SCANCODE_D, SDL_SCANCODE_RIGHT };

    // todo - figure out if these feel right
    keyboardBindings[KeyBinding::A] = { SDL_SCANCODE_X, SDL_SCANCODE_K };
    keyboardBindings[KeyBinding::B] = { SDL_SCANCODE_Z, SDL_SCANCODE_J };

    keyboardBindings[KeyBinding::Start] = { SDL_SCANCODE_RETURN, SDL_SCANCODE_KP_ENTER };
    keyboardBindings[KeyBinding::Select] = { SDL_SCANCODE_LSHIFT }; // todo - don't think this will automatically get picked up the way we've set it up

    // gamepad controls

    gamepadBindings[KeyBinding::Up] = { SDL_CONTROLLER_BUTTON_DPAD_UP };
    gamepadBindings[KeyBinding::Down] = { SDL_CONTROLLER_BUTTON_DPAD_DOWN };
    gamepadBindings[KeyBinding::Left] = { SDL_CONTROLLER_BUTTON_DPAD_LEFT };
    gamepadBindings[KeyBinding::Right] = { SDL_CONTROLLER_BUTTON_DPAD_RIGHT };

    gamepadBindings[KeyBinding::A] = { SDL_CONTROLLER_BUTTON_A };
    gamepadBindings[KeyBinding::B] = { SDL_CONTROLLER_BUTTON_B };

    gamepadBindings[KeyBinding::Start] = { SDL_CONTROLLER_BUTTON_START };
    gamepadBindings[KeyBinding::Select] = { SDL_CONTROLLER_BUTTON_TOUCHPAD, SDL_CONTROLLER_BUTTON_BACK }; // todo - might be one of these need to test
}

