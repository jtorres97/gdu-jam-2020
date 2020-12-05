#include "Game.h"

void Game::Initialize() 
{
    SDL_Log("Initializing game...", SDL_LOG_PRIORITY_INFO);

    m_renderer.Initialize();
}

void Game::Run() 
{
    SDL_Log("Starting the game loop...", SDL_LOG_PRIORITY_INFO);

    m_isRunning = true;

    auto previousTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> lag{};

    while (m_isRunning) 
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = currentTime - previousTime;
        lag += elapsedTime;

        m_inputState.PollForInput();

        while (lag >= TIME_PER_TICK)
        {
            // TODO: Update game state based on user input
            // TODO: Update game objects

            if (m_inputState.quit)
            {
                m_isRunning = false;
            }

            lag -= TIME_PER_TICK;
        }

        m_renderer.Clear();

        // TODO: Render stuff

        // Draw cursor
        m_renderer.RenderRectangle({ m_inputState.cursor, float(TEXTURE_SCALE)}, FG_COLOR.r, FG_COLOR.g, FG_COLOR.b, FG_COLOR.r);
        
        m_renderer.Present();
    }
}

void Game::Close()
{
    SDL_Log("Closing game...", SDL_LOG_PRIORITY_INFO);
}