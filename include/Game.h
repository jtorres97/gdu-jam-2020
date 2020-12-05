#pragma once

#include <memory>
#include <chrono>
#include <vector>

#include "Renderer.h"
#include "GameState.h"
#include "Player.h"
#include "Spawner.h"
#include "Enemy.h"
#include "Projectile.h"

const double TICK_RATE = 60;
const auto TIME_PER_TICK = std::chrono::duration<double>(1.0 / TICK_RATE);

class Game
{
public:
    Game();

    void Initialize();
    void Run();
    void Close();

private:
    void Update();
    void Render();
    void Cleanup();

    void HandlePlayerCollisions(std::shared_ptr<Projectile> projectile);
    void HandleEnemyCollisions(std::shared_ptr<Projectile> projectile);
    void HandleSpawnerCollisions(std::shared_ptr<Projectile> projectile);

    bool m_isRunning;
    GameState m_state;
    SDLRenderer m_renderer;
    std::shared_ptr<Player> m_playerOne;
    std::vector<std::shared_ptr<Spawner>> m_spawners;
    std::vector<std::shared_ptr<Enemy>> m_enemies;
    std::vector<std::shared_ptr<Projectile>> m_projectiles;
    std::vector<std::shared_ptr<Projectile>> m_playerProjectiles;
    Timer m_cleanupTimer;
};