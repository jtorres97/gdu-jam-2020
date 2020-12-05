#pragma once

#include <memory>
#include <vector>

#include "UserInput.h"

enum class GameStatus
{
    UNKNOWN,
    RUNNING,
    GAME_OVER
};

class GameState
{
public:
    GameStatus status = GameStatus::UNKNOWN;

    GameState();

    UserInputState& GetInputState();

    void SetPlayerPosition(Point position);
    Point GetPlayerPosition();

private:
    UserInputState m_inputState;
    Point m_playerPosition;
};