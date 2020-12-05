#pragma once

#include "UserInput.h"

class GameState
{
public:
    GameState();

    UserInputState& GetInputState();

private:
    UserInputState m_inputState;
};