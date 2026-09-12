#include "Input.h"

Input& Input::only()
{
    static Input input;
    return input;
}

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
        // process gamepad bindings
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
    for (auto& scanCode : keyboardBindings.at(binding))
    {
        if (currentKeyboardState[scanCode]) return true;
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

