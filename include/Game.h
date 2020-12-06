#pragma once

#include <memory>
#include <chrono>
#include <vector>

#include "Renderer.h"
#include "GameState.h"
#include "Player.h"
#include "Enemy.h"
#include "Font.h"
#include "Database.h"

const double TICK_RATE = 60;
const auto TIME_PER_TICK = std::chrono::duration<double>(1.0 / TICK_RATE);

enum class GameDifficulty
{
    EASY = 0,
    NORMAL = 1,
    HARD = 2,
};

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
    void Reset();

    bool m_isRunning;
    GameState m_state;
    SDLRenderer m_renderer;
    Database m_DB;

    std::shared_ptr<Font> m_UIFont;

    std::shared_ptr<Texture> m_overlayTexture;
    std::shared_ptr<Texture> m_enemyTexture;

    std::shared_ptr<Player> m_playerOne;
    std::vector<std::shared_ptr<Enemy>> m_enemies;

    GameDifficulty m_selectedDifficulty = GameDifficulty::NORMAL;

    int startingTimeout = 1000;
    Timer m_fireTimer;
    Timer m_cleanupTimer;
    EnemyDirection m_lastDirection = EnemyDirection::UNKNOWN;
};