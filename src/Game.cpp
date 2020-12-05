#include "Game.h"
#include "Player.h"
#include "Spawner.h"

#include <algorithm>

Game::Game() : m_cleanupTimer(5000)
{
    m_cleanupTimer.Reset();
}

void Game::Initialize()
{
    SDL_Log("Initializing game...", SDL_LOG_PRIORITY_INFO);

    m_renderer.Initialize();

    m_UIFont = m_renderer.LoadFont("assets/EXEPixelPerfect.ttf");

    // Add player
    m_playerOne = std::make_shared<Player>();
    m_playerOne->SetMainTexture(m_renderer.LoadTexture("assets/player.png"));

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

        m_state.GetInputState().PollForInput();

        while (lag >= TIME_PER_TICK)
        {
            Update();

            if (m_cleanupTimer.IsExpired())
            {
                Cleanup();
                m_cleanupTimer.Reset();
            }

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
    if (m_state.status != GameStatus::RUNNING)
        return;

    // Update game objects

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

        Point n = p->GetPosition();
        if (n.x < 0 || n.x > WORLDSIZE_W || n.y < 0 || n.y > WORLDSIZE_H)
        {
            p->Deactivate();
        }
    }

    // Collision checks

    for (auto p : m_projectiles)
    {
        if (p->IsActive())
        {
            if (p->friendly)
            {
                HandleEnemyCollisions(p);
                HandleSpawnerCollisions(p);
            }
            else
            {
                HandlePlayerCollisions(p);
            }
        }
    }

    // TODO: Check for player <---> enemy collisions

    // Check if player is still alive
    if (!m_playerOne->IsAlive())
    {
        m_state.status = GameStatus::GAME_OVER;
    }

    // Move any created projectiles to the main list
    m_projectiles.insert(m_projectiles.end(),
                       std::make_move_iterator(m_playerProjectiles.begin()),
                       std::make_move_iterator(m_playerProjectiles.end()));
    m_playerProjectiles.clear();
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

    // Render the game over screen
    if (m_state.status == GameStatus::GAME_OVER)
    {
        Rectangle textRect = {(WORLDSIZE_W - 600) / 2, (WORLDSIZE_H - 100) / 2, 600, 100};
        m_renderer.RenderFont(m_UIFont, "GAME OVER", textRect);
    }

    // Render cursor
    Point cursorPosition = m_state.GetInputState().cursor;
    m_renderer.RenderRectangle({cursorPosition, float(TEXTURE_SCALE), float(TEXTURE_SCALE)}, FG_COLOR.r, FG_COLOR.g, FG_COLOR.b, FG_COLOR.r);

    m_renderer.Present();
}

void Game::Cleanup()
{
    // Cleanup deactivated objects

    for (int i = 0; i < m_spawners.size(); i++)
    {
        m_spawners.erase(
            std::remove_if(m_spawners.begin(),
                           m_spawners.end(),
                           [](std::shared_ptr<Spawner> o) { return !o->IsActive(); }),
            m_spawners.end());
    }

    for (int i = 0; i < m_enemies.size(); i++)
    {
        m_enemies.erase(
            std::remove_if(m_enemies.begin(),
                           m_enemies.end(),
                           [](std::shared_ptr<Enemy> o) { return !o->IsActive(); }),
            m_enemies.end());
    }

    for (int i = 0; i < m_projectiles.size(); i++)
    {
        m_projectiles.erase(
            std::remove_if(m_projectiles.begin(),
                           m_projectiles.end(),
                           [](std::shared_ptr<Projectile> o) { return !o->IsActive(); }),
            m_projectiles.end());
    }
}

void Game::Reset()
{
    m_spawners.clear();
    m_enemies.clear();
    m_projectiles.clear();

    // Add enemy spawner
    auto spawner0 = std::make_shared<Spawner>(Point{100, 600}, m_enemies, m_projectiles);
    spawner0->SetMainTexture(m_renderer.LoadTexture("assets/spawner.png"));
    spawner0->SetEnemyTexture(m_renderer.LoadTexture("assets/enemy.png"));
    m_spawners.push_back(spawner0);

    auto spawner1 = std::make_shared<Spawner>(Point{800, 200}, m_enemies, m_projectiles);
    spawner1->SetMainTexture(m_renderer.LoadTexture("assets/spawner.png"));
    spawner1->SetEnemyTexture(m_renderer.LoadTexture("assets/enemy.png"));
    m_spawners.push_back(spawner1);
}

void Game::HandlePlayerCollisions(std::shared_ptr<Projectile> projectile)
{
    if (RectangleCollision(m_playerOne->GetHitBox(), projectile->GetHitBox()))
    {
        projectile->Deactivate();

        // Determine angle of hit
        float projectileAngle = projectile->GetVelocity().GetAngle();

        if (projectileAngle < 0.0)
        {
            projectileAngle += Pi;
        }
        else
        {
            projectileAngle -= Pi;
        }

        // If player angle is between -45 and 45 degrees of the projectile angle, reflect, else damage
        float minAngle = projectileAngle - Pi / 4;
        float maxAngle = projectileAngle + Pi / 4;
        if (m_playerOne->GetRotation() > minAngle &&
            m_playerOne->GetRotation() < maxAngle)
        {
            if (m_playerOne->GetFlip())
            {
                // refract projectiles
                for (int i = 0; i < 6; i++)
                {
                    // TODO: fix this refraction math
                    Vector2D refractionVector = Vector2D(10.0, projectile->GetVelocity().GetAngle() + ((i * projectile->GetVelocity().GetAngle()) / 6));
                    auto refractedProjectile = std::make_shared<Projectile>(m_playerOne->GetHitBox().pos, refractionVector);
                    refractedProjectile->friendly = true;

                    m_playerProjectiles.push_back(refractedProjectile);
                }
            }
            else
            {
                // reflect projectile
                Vector2D reflectionVector = Vector2D(10, m_playerOne->GetRotation());
                auto reflectedProjectile = std::make_shared<Projectile>(m_playerOne->GetPosition(), reflectionVector);
                reflectedProjectile->friendly = true;

                m_playerProjectiles.push_back(reflectedProjectile);
            }
        }
        else
        {
            m_playerOne->Damage(10);
        }
    }
}

void Game::HandleEnemyCollisions(std::shared_ptr<Projectile> projectile)
{
    for (auto e : m_enemies)
    {
        if (RectangleCollision(projectile->GetHitBox(), e->GetHitBox()))
        {
            projectile->Deactivate();

            e->Damage(10);
        }
    }
}

void Game::HandleSpawnerCollisions(std::shared_ptr<Projectile> projectile)
{
    for (auto s : m_spawners)
    {
        if (RectangleCollision(projectile->GetHitBox(), s->GetHitBox()))
        {
            projectile->Deactivate();

            s->Damage(10);
        }
    }
}