#pragma once

#include "UserInput.h"

class GameState
{
public:
    GameState();

    UserInputState& GetInputState();

    void SetPlayerPosition(Point position);
    Point GetPlayerPosition();

private:
    UserInputState m_inputState;
    Point m_playerPosition;
};