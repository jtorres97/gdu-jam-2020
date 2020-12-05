#include <SDL2/SDL.h>

#include "UserInput.h"
#include "Settings.h"

void UserInputState::PollForInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            quit = true;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_w:
            case SDLK_UP:
                up = true;
                break;
            case SDLK_s:
            case SDLK_DOWN:
                down = true;
                break;
            case SDLK_a:
            case SDLK_LEFT:
                left = true;
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                right = true;
                break;
            case SDLK_RETURN:
            case SDLK_SPACE:
                select = true;
                break;
            }
        }
        else if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_w:
            case SDLK_UP:
                up = false;
                break;
            case SDLK_s:
            case SDLK_DOWN:
                down = false;
                break;
            case SDLK_a:
            case SDLK_LEFT:
                left = false;
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                right = false;
                break;
            case SDLK_RETURN:
            case SDLK_SPACE:
                select = false;
                break;
            }
        }
    }
}

Vector2D WASDToMovementVector(bool U, bool D, bool L, bool R)
{
    float magnitude = 1.0;
    float angle = 0.0;

    if ((R && L) || U && D)
    {
        magnitude = 0.0;
    }
    // else if (R && U)
    // {
    //     angle = 45.0 * TO_RADS;
    // }
    // else if (L && U)
    // {
    //     angle = 135.0 * TO_RADS;
    // }
    // else if (L && D)
    // {
    //     angle = 225.0 * TO_RADS;
    // }
    // else if (R && D)
    // {
    //     angle = 315.0 * TO_RADS;
    // }
    else if (R)
    {
        angle = 0.0 * TO_RADS;
    }
    else if (U)
    {
        angle = 90.0 * TO_RADS;
    }
    else if (L)
    {
        angle = 180.0 * TO_RADS;
    }
    else if (D)
    {
        angle = -90.0 * TO_RADS;
    }
    else
    {
        magnitude = 0.0;
        angle = 0.0;
    }

    return Vector2D(magnitude, angle);
}