#pragma once

#include <memory>
#include <vector>

#include "UserInput.h"

enum class GameStatus
{
    UNKNOWN,
    MENU,
    RUNNING,
    GAME_OVER
};

struct GameState
{
    GameStatus status = GameStatus::UNKNOWN;
    int score = 0;
    int bestScore = 0;
    UserInputState input;
};