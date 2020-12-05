#pragma once

#include "Geometry.h"
#include "Vector.h"

struct UserInputState
{
    bool quit = false;
    Point cursor;

    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    bool select = false;

    void PollForInput();
};

// Converts a combination of directions into a unit vector for movement at that angle
Vector2D WASDToMovementVector(bool U, bool D, bool L, bool R);