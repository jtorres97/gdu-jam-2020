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
#include "Animation.h"

const double TICK_RATE = 60;
const auto TIME_PER_TICK = std::chrono::duration<double>(1.0 / TICK_RATE);

const int EASY_STARTING_RATE = 1000;
const float EASY_MOVE_SPEED = 3.0;
const int EASY_RATE_INCREASE = 20;
const int EASY_RATE_MIN = 400;

const int NORMAL_STARTING_RATE = 800;
const float NORMAL_MOVE_SPEED = 4.0;
const int NORMAL_RATE_INCREASE = 30;
const int NORMAL_RATE_MIN = 300;

const int INSANE_STARTING_RATE = 700;
const float INSANE_MOVE_SPEED = 5.0;
const int INSANE_RATE_INCREASE = 50;
const int INSANE_RATE_MIN = 250;

enum class GameDifficulty
{
    EASY = 0,
    NORMAL = 1,
    INSANE = 2,
};

struct DifficultySettings
{
    GameDifficulty difficulty;
    int startingRate;
    float moveSpeed;
    int rateIncrease;
    int rateTimeoutMin;
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
    std::shared_ptr<Texture> m_scoreFrameTexture;
    std::shared_ptr<Texture> m_titleLogo;

    std::shared_ptr<Player> m_playerOne;
    std::vector<std::shared_ptr<Enemy>> m_enemies;

    DifficultySettings m_selectedDifficulty;

    int startingTimeout = 1000;
    Timer m_fireTimer;
    Timer m_cleanupTimer;
    EnemyDirection m_lastDirection = EnemyDirection::UNKNOWN;

    // UI stuff
    Animation m_buttonEasyAnimation;
    Animation m_buttonNormalAnimation;
    Animation m_buttonInsaneAnimation;
    Animation m_buttonExitAnimation;
    Animation m_buttonExitLargeAnimation;
    Animation m_buttonRetryAnimation;
    Animation m_mainMenuAnimation;

    int m_mainMenuSelectedButtonIndex = 0;
    int m_gameoverMenuSelectedButtonIndex = 0;
    int m_pauseMenuSelectedButtonIndex = 0;

    bool m_menuButtonDownLast = false;
    bool m_modeSwitch = false;
    bool m_highScoreFlag = false;

    // Sounds
    std::shared_ptr<Sound> m_positiveSound;
    std::shared_ptr<Sound> m_negativeSound;
    std::shared_ptr<Sound> m_clickSound;
    std::shared_ptr<Sound> m_selectSound;
    std::shared_ptr<Sound> m_highScoreSound;

    std::shared_ptr<Music> m_easyMusic;
    std::shared_ptr<Music> m_mediumMusic;
    std::shared_ptr<Music> m_insaneMusic;
};