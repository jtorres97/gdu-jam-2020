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
    int score = 0;
    int level = 1;

    GameState();

    UserInputState& GetInputState();

private:
    UserInputState m_inputState;
};