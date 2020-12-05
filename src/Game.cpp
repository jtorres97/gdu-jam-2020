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

    m_renderer.Initialize();

    // Load assets
    m_UIFont = m_renderer.LoadFont("assets/EXEPixelPerfect.ttf");
    m_overlayTexture = m_renderer.LoadTexture("assets/overlay.png");
    m_enemyTexture = m_renderer.LoadTexture("assets/enemy.png");
    auto playerTexture = m_renderer.LoadTexture("assets/player.png");

    // Add player
    m_playerOne = std::make_shared<Player>();
    m_playerOne->SetMainTexture(playerTexture);

    Reset();
}

void Game::Run()
{
    SDL_Log("Starting the game loop...", SDL_LOG_PRIORITY_INFO);

    m_isRunning = true;
    m_state.status = GameStatus::RUNNING;

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
    if (m_state.status != GameStatus::RUNNING)
    {
        if (m_state.input.select)
        {
            Reset();
            m_state.status = GameStatus::RUNNING;
        }
        return;
    }

    // Update game objects

    m_playerOne->Update(m_state);

    if (m_fireTimer.IsExpired())
    {
        // Fire from a random direction
        Point p = {400, -20};
        EnemyDirection dir = EnemyDirection::UP;

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

        auto e = std::make_shared<Enemy>(p);
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

            // this is gross
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
            }

            e->Deactivate();
        }
    }
}

void Game::Render()
{
    m_renderer.Clear();

    // Render objects
    m_playerOne->Render(m_renderer);

    for (auto e : m_enemies)
    {
        e->Render(m_renderer);
    }

    m_renderer.RenderWholeTexture(m_overlayTexture, {0, 0, 800, 800});

    // Render score
    Rectangle scoreRect = {20, (WORLDSIZE_H - 60) - 20, 80, 60};
    m_renderer.RenderFont(m_UIFont, std::to_string(m_state.score), scoreRect);

    Rectangle bestScoreRect = {(WORLDSIZE_W - 80) - 20, (WORLDSIZE_H - 60) - 20, 80, 60};
    m_renderer.RenderFont(m_UIFont, std::to_string(m_state.bestScore), bestScoreRect);

    // Render the game over screen
    if (m_state.status == GameStatus::GAME_OVER)
    {
        Rectangle textRect = {100, 350, 600, 100};
        Rectangle outline = {80, 350, 620, 100};

        m_renderer.RenderRectangle(outline, FG_COLOR.r, FG_COLOR.g, FG_COLOR.b, FG_COLOR.a);
        m_renderer.RenderFont(m_UIFont, "TRY AGAIN?", textRect);
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
    if (m_state.score > m_state.bestScore)
    {
        // Update best score
        m_state.bestScore = m_state.score;
    }

    m_fireTimer.SetTimeout(1000);
    m_fireTimer.Reset();

    m_enemies.clear();
}