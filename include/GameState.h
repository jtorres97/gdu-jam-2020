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
    UserInputState input;

    int score = 0;
    int bestScore = 0;
    int bestScoreEasy = 0;
    int bestScoreNormal = 0;
    int bestScoreInsane = 0;
};