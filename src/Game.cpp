#include "Game.h"
#include "Player.h"
#include "Spawner.h"

#include <algorithm>

void Game::Initialize() 
{
    SDL_Log("Initializing game...", SDL_LOG_PRIORITY_INFO);

    m_renderer.Initialize();

    // Add player
    auto player = std::make_shared<Player>();
    player->SetMainTexture(m_renderer.LoadTexture("assets/player.png"));
    m_playerOne = player;

    // Add enemy spawners
    auto spawner0 = std::make_shared<Spawner>(Point{100, 600}, m_enemies, m_projectiles);
    spawner0->SetMainTexture(m_renderer.LoadTexture("assets/spawner.png"));
    spawner0->SetEnemyTexture(m_renderer.LoadTexture("assets/enemy.png"));
    m_spawners.push_back(spawner0);

    auto spawner1 = std::make_shared<Spawner>(Point{900, 600}, m_enemies, m_projectiles);
    spawner1->SetMainTexture(m_renderer.LoadTexture("assets/spawner.png"));
    spawner1->SetEnemyTexture(m_renderer.LoadTexture("assets/enemy.png"));
    m_spawners.push_back(spawner1);
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
        previousTime = currentTime;
        lag += elapsedTime;

        m_state.GetInputState().PollForInput();

        while (lag >= TIME_PER_TICK)
        {
            Update();

            // Cleanup in-active entities
            // for (int i = 0; i < m_entities.size(); i++)
            // {
            //     m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [](std::shared_ptr<Entity> e) { return !e->IsActive(); }), m_entities.end());
            // }
 
            if (m_state.GetInputState().quit)
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
    m_playerOne->Update(m_state);

    for (auto s : m_spawners)
    {
        s->Update(m_state);
    }

    for (auto e : m_enemies)
    {
        e->Update(m_state);
    }

    for (auto p : m_projectiles)
    {
        p->Update(m_state);
    }
}

void Game::Render()
{
    m_renderer.Clear();

    // Render objects
    m_playerOne->Render(m_renderer);

    for (auto s : m_spawners)
    {
        s->Render(m_renderer);
    }

    for (auto e : m_enemies)
    {
        e->Render(m_renderer);
    }

    for (auto p : m_projectiles)
    {
        p->Render(m_renderer);
    }

    // Render cursor
    Point cursorPosition = m_state.GetInputState().cursor;
    m_renderer.RenderRectangle({cursorPosition, float(TEXTURE_SCALE), float(TEXTURE_SCALE)}, FG_COLOR.r, FG_COLOR.g, FG_COLOR.b, FG_COLOR.r);

    m_renderer.Present();
}