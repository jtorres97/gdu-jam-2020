#pragma once

#include "Geometry.h"

struct UserInputState
{
    bool quit = false;
    Point cursor;

    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    bool fireMain = false;
    bool fireSecondary = false;

    void PollForInput();
};