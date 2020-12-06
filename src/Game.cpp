#include "Game.h"
#include "Player.h"
#include "Random.h"

#include <algorithm>

Game::Game() : m_cleanupTimer(5000), m_fireTimer(1000)
{
    m_cleanupTimer.Reset();
    m_fireTimer.Reset();
}

void Game::Initialize()
{
    SDL_Log("Initializing game...", SDL_LOG_PRIORITY_INFO);

    m_DB.Open();

    m_renderer.Initialize();

    // Load fonts and textures
    m_UIFont = m_renderer.LoadFont("assets/EXEPixelPerfect.ttf");
    m_overlayTexture = m_renderer.LoadTexture("assets/overlay.png");
    m_enemyTexture = m_renderer.LoadTexture("assets/enemy.png");
    auto playerTexture = m_renderer.LoadTexture("assets/player.png");

    // load animations
    ButtonCasualAnimation = Animation(m_renderer.LoadTexture("assets/button_easy.png"), 2, 47, 12, 0, false);
    ButtonNormalAnimation = Animation(m_renderer.LoadTexture("assets/button_normal.png"), 2, 47, 12, 0, false);
    ButtonInsaneAnimation = Animation(m_renderer.LoadTexture("assets/button_insane.png"), 2, 47, 12, 0, false);
    ButtonExitAnimation = Animation(m_renderer.LoadTexture("assets/button_exit.png"), 2, 33, 12, 0, false);
    ButtonExitLargeAnimation = Animation(m_renderer.LoadTexture("assets/button_exit_large.png"), 2, 47, 12, 0, false);
    ButtonRetryAnimation = Animation(m_renderer.LoadTexture("assets/button_retry.png"), 2, 47, 12, 0, false);

    // Add player
    m_playerOne = std::make_shared<Player>();
    m_playerOne->SetMainTexture(playerTexture);

    Reset();
}

void Game::Run()
{
    SDL_Log("Starting the game loop...", SDL_LOG_PRIORITY_INFO);

    m_isRunning = true;
    m_state.status = GameStatus::MENU;

    auto previousTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> lag{};

    while (m_isRunning)
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = currentTime - previousTime;
        previousTime = currentTime;
        lag += elapsedTime;

        m_state.input.PollForInput();

        while (lag >= TIME_PER_TICK)
        {
            Update();

            if (m_cleanupTimer.IsExpired())
            {
                Cleanup();
                m_cleanupTimer.Reset();
            }

            if (m_state.input.quit)
            {
                m_isRunning = false;
            }

            lag -= TIME_PER_TICK;
        }
        Render();
    }
}

void Game::Close()
{
    SDL_Log("Closing game...", SDL_LOG_PRIORITY_INFO);
}

void Game::Update()
{
    if (m_state.status == GameStatus::MENU)
    {
        if (ModeSwitch && m_state.input.select)
            return;

        ModeSwitch = false;

        // Change the selected button index
        if (m_state.input.down)
        {
            if (!MenuButtonDownLast)
            {
                MainMenuSelectedButtonIndex = MainMenuSelectedButtonIndex == 3 ? 0 : MainMenuSelectedButtonIndex + 1;
                MenuButtonDownLast = true;
            }
        }
        else if (m_state.input.up)
        {
            if (!MenuButtonDownLast)
            {
                MainMenuSelectedButtonIndex = MainMenuSelectedButtonIndex == 0 ? 3 : MainMenuSelectedButtonIndex - 1;
                MenuButtonDownLast = true;
            }
        }
        else
        {
            MenuButtonDownLast = false;
        }

        // Reset all buttons
        ButtonCasualAnimation.SetFrame(0);
        ButtonNormalAnimation.SetFrame(0);
        ButtonInsaneAnimation.SetFrame(0);
        ButtonExitAnimation.SetFrame(0);

        if (MainMenuSelectedButtonIndex == 0)
        {
            ButtonCasualAnimation.SetFrame(1);
        }
        else if (MainMenuSelectedButtonIndex == 1)
        {
            ButtonNormalAnimation.SetFrame(1);
        }
        else if (MainMenuSelectedButtonIndex == 2)
        {
            ButtonInsaneAnimation.SetFrame(1);
        }
        else if (MainMenuSelectedButtonIndex == 3)
        {
            ButtonExitAnimation.SetFrame(1);
        }

        if (m_state.input.select)
        {
            // Button selected, handle it
            if (MainMenuSelectedButtonIndex == 0)
            {
                // Play easy
                m_selectedDifficulty = GameDifficulty::EASY;
                Reset();
                m_state.status = GameStatus::RUNNING;
            }
            else if (MainMenuSelectedButtonIndex == 1)
            {
                // Play normal
                m_selectedDifficulty = GameDifficulty::NORMAL;
                Reset();
                m_state.status = GameStatus::RUNNING;
            }
            else if (MainMenuSelectedButtonIndex == 2)
            {
                // Play insane
                m_selectedDifficulty = GameDifficulty::INSANE;
                Reset();
                m_state.status = GameStatus::RUNNING;
            }
            else if (MainMenuSelectedButtonIndex == 3)
            {
                // Exit
                m_isRunning = false;
            }
        }
    }
    else if (m_state.status == GameStatus::GAME_OVER)
    {
        // Change the selected button index
        if (m_state.input.down)
        {
            if (!MenuButtonDownLast)
            {
                GameoverMenuSelectedButtonIndex = GameoverMenuSelectedButtonIndex == 1 ? 0 : GameoverMenuSelectedButtonIndex + 1;
                MenuButtonDownLast = true;
            }
        }
        else if (m_state.input.up)
        {
            if (!MenuButtonDownLast)
            {
                GameoverMenuSelectedButtonIndex = GameoverMenuSelectedButtonIndex == 0 ? 1 : GameoverMenuSelectedButtonIndex - 1;
                MenuButtonDownLast = true;
            }
        }
        else
        {
            MenuButtonDownLast = false;
        }

        // Reset all buttons
        ButtonRetryAnimation.SetFrame(0);
        ButtonExitLargeAnimation.SetFrame(0);

        if (GameoverMenuSelectedButtonIndex == 0)
        {
            ButtonRetryAnimation.SetFrame(1);
        }
        else if (GameoverMenuSelectedButtonIndex == 1)
        {
            ButtonExitLargeAnimation.SetFrame(1);
        }

        if (m_state.input.select)
        {
            // Button selected, handle it
            if (GameoverMenuSelectedButtonIndex == 0)
            {
                // Handle retry
                Reset();
                m_state.status = GameStatus::RUNNING;
            }
            else if (GameoverMenuSelectedButtonIndex == 1)
            {
                // Handle exit
                m_state.status = GameStatus::MENU;
                ModeSwitch = true;
            }
        }
    }
    else if (m_state.status == GameStatus::RUNNING)
    {
        // Update game objects

        m_playerOne->Update(m_state);

        if (m_fireTimer.IsExpired())
        {
            // Fire from a random direction
            Point p;
            EnemyDirection dir = m_lastDirection;
            std::shared_ptr<Enemy> e = nullptr;

            // Never repeat directions
            while (dir == m_lastDirection)
            {
                int rand = RandInt(4);

                if (rand == 0)
                {
                    p = {-20, 400};
                    dir = EnemyDirection::RIGHT;
                }
                else if (rand == 1)
                {
                    p = {400, 820};
                    dir = EnemyDirection::DOWN;
                }
                else if (rand == 2)
                {
                    p = {820, 400};
                    dir = EnemyDirection::LEFT;
                }
                else if (rand == 3)
                {
                    p = {400, -20};
                    dir = EnemyDirection::UP;
                }
            }

            m_lastDirection = dir;

            e = std::make_shared<Enemy>(p);
            e->SetMainTexture(m_enemyTexture);
            e->direction = dir;

            m_enemies.push_back(e);

            // Update rate as level increases
            if (m_state.score % 10 && m_state.score < 80)
            {
                m_fireTimer.SetTimeout(startingTimeout - m_state.score * 10);
            }

            m_fireTimer.Reset();
        }

        for (auto e : m_enemies)
        {
            e->Update(m_state);
        }

        // Check for collisions
        for (auto e : m_enemies)
        {
            if (e->IsActive() && RectangleCollision(m_playerOne->GetHitBox(), e->GetHitBox()))
            {
                EnemyDirection d = e->direction;
                float rot = m_playerOne->GetRotation();
                if ((d == EnemyDirection::LEFT && CloseEnough(rot, 0.0)) ||
                    (d == EnemyDirection::UP && CloseEnough(rot, -Pi / 2)) ||
                    (d == EnemyDirection::RIGHT && CloseEnough(rot, Pi)) ||
                    (d == EnemyDirection::DOWN && CloseEnough(rot, Pi / 2)))
                {
                    m_state.score++;
                }
                else
                {
                    m_state.status = GameStatus::GAME_OVER;

                    if (m_state.score > m_state.bestScore)
                    {
                        // Update best score
                        m_state.bestScore = m_state.score;

                        m_DB.AddScore(static_cast<int>(m_selectedDifficulty), m_state.score);
                    }
                }

                e->Deactivate();
            }
        }
    }
}

void Game::Render()
{
    m_renderer.Clear();

    if (m_state.status == GameStatus::MENU)
    {
        // TODO: Render title

        // Render buttons
        ButtonCasualAnimation.Render(m_renderer, 18 * TEXTURE_SCALE, 57 * TEXTURE_SCALE);
        ButtonNormalAnimation.Render(m_renderer, 18 * TEXTURE_SCALE, 42 * TEXTURE_SCALE);
        ButtonInsaneAnimation.Render(m_renderer, 18 * TEXTURE_SCALE, 27 * TEXTURE_SCALE);
        ButtonExitAnimation.Render(m_renderer, 35 * TEXTURE_SCALE, 10 * TEXTURE_SCALE);
    }
    else if (m_state.status == GameStatus::GAME_OVER)
    {
        m_renderer.RenderWholeTexture(m_overlayTexture, {0, 0, 800, 800});

        // Render score
        float w = 120;
        float h = 60;
        float offset = 20;
        Rectangle scoreRect = {offset, WORLDSIZE_H - h - offset, w, h};
        m_renderer.RenderFont(m_UIFont, "SCORE: " + std::to_string(m_state.score), scoreRect);

        Rectangle bestScoreRect = {WORLDSIZE_W - w - offset, WORLDSIZE_H - h - offset, w, h};
        m_renderer.RenderFont(m_UIFont, "BEST: " + std::to_string(m_state.bestScore), bestScoreRect);

        // Render buttons

        ButtonRetryAnimation.Render(m_renderer, 18 * TEXTURE_SCALE, 50 * TEXTURE_SCALE);
        ButtonExitLargeAnimation.Render(m_renderer, 18 * TEXTURE_SCALE, 25 * TEXTURE_SCALE);
    }
    else if (m_state.status == GameStatus::RUNNING)
    {
        m_playerOne->Render(m_renderer);

        for (auto e : m_enemies)
        {
            e->Render(m_renderer);
        }

        m_renderer.RenderWholeTexture(m_overlayTexture, {0, 0, 800, 800});

        // Render score
        float w = 120;
        float h = 60;
        float offset = 20;
        Rectangle scoreRect = {offset, WORLDSIZE_H - h - offset, w, h};
        m_renderer.RenderFont(m_UIFont, "SCORE: " + std::to_string(m_state.score), scoreRect);

        Rectangle bestScoreRect = {WORLDSIZE_W - w - offset, WORLDSIZE_H - h - offset, w, h};
        m_renderer.RenderFont(m_UIFont, "BEST: " + std::to_string(m_state.bestScore), bestScoreRect);
    }

    m_renderer.Present();
}

void Game::Cleanup()
{
    // Cleanup deactivated objects
    for (int i = 0; i < m_enemies.size(); i++)
    {
        m_enemies.erase(
            std::remove_if(m_enemies.begin(),
                           m_enemies.end(),
                           [](std::shared_ptr<Enemy> o) { return !o->IsActive(); }),
            m_enemies.end());
    }
}

void Game::Reset()
{
    m_state.score = 0;
    m_state.bestScore = m_DB.GetHighScore(static_cast<int>(m_selectedDifficulty));

    m_fireTimer.SetTimeout(1000);
    m_fireTimer.Reset();

    m_enemies.clear();
}