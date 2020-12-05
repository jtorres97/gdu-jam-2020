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
        else if (event.type == SDL_MOUSEMOTION)
        {
            // Continuously capture mouse position
            int x, y;
            SDL_GetMouseState(&x, &y);
            cursor.x = x * (WORLDSIZE_W / RESOLUTION_W);
            cursor.y = (RESOLUTION_H - y) * (WORLDSIZE_H / RESOLUTION_H);
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                fireMain = true;
                break;
            case SDL_BUTTON_RIGHT:
                break;
            default:
                break;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                fireMain = false;
                break;
            case SDL_BUTTON_RIGHT:
                break;
            default:
                break;
            }
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_w:
                up = true;
                break;
            case SDLK_s:
                down = true;
                break;
            case SDLK_a:
                left = true;
                break;
            case SDLK_d:
                right = true;
                break;
            case SDLK_RETURN:
                fireMain = true;
                break;
            }
        }
        else if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_w:
                up = false;
                break;
            case SDLK_s:
                down = false;
                break;
            case SDLK_a:
                left = false;
                break;
            case SDLK_d:
                right = false;
                break;
            case SDLK_RETURN:
                fireMain = false;
                break;
            }
        }
    }
}