#include "Game.h"
#include "Player.h"
#include "Random.h"

#include <algorithm>

std::shared_ptr<Animation> CreateVirusAnimation(std::shared_ptr<Texture> spriteSheet)
{
    return std::make_shared<Animation>(spriteSheet, 8, 28, 28, 80, false);
}

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
    m_enemyTexture = m_renderer.LoadTexture("assets/virus.png");
    m_scoreFrameTexture = m_renderer.LoadTexture("assets/score_frame.png");
    m_titleLogo = m_renderer.LoadTexture("assets/logo.png");

    // load animations
    m_buttonEasyAnimation = Animation(m_renderer.LoadTexture("assets/button_easy.png"), 2, 47 * 4, 12 * 4, 0, false);
    m_buttonNormalAnimation = Animation(m_renderer.LoadTexture("assets/button_normal.png"), 2, 47 * 4, 12 * 4, 0, false);
    m_buttonInsaneAnimation = Animation(m_renderer.LoadTexture("assets/button_insane.png"), 2, 47 * 4, 12 * 4, 0, false);
    m_buttonExitAnimation = Animation(m_renderer.LoadTexture("assets/button_exit.png"), 2, 33 * 4, 12 * 4, 0, false);
    m_buttonExitLargeAnimation = Animation(m_renderer.LoadTexture("assets/button_exit_large.png"), 2, 47 * 4, 12 * 4, 0, false);
    m_buttonRetryAnimation = Animation(m_renderer.LoadTexture("assets/button_retry.png"), 2, 47 * 4, 12 * 4, 0, false);
    m_mainMenuAnimation = Animation(m_renderer.LoadTexture("assets/player_down.png"), 3, 28, 28, 150, true);

    // Load sounds
    m_positiveSound = m_renderer.LoadSFX("assets/positive.wav");
    m_negativeSound = m_renderer.LoadSFX("assets/negative.wav");
    m_clickSound = m_renderer.LoadSFX("assets/move.wav");
    m_selectSound = m_renderer.LoadSFX("assets/select.wav");
    m_highScoreSound = m_renderer.LoadSFX("assets/highscore.wav");

    // load music
    m_easyMusic = m_renderer.LoadMusic("assets/easy_theme.mp3");
    m_mediumMusic = m_renderer.LoadMusic("assets/normal_theme.mp3");
    m_insaneMusic = m_renderer.LoadMusic("assets/insane_theme.mp3");

    // Add player
    m_playerOne = std::make_shared<Player>();
    m_playerOne->SetRightAnimation(std::make_shared<Animation>(m_renderer.LoadTexture("assets/player_right.png"), 3, 28, 28, 150, true));
    m_playerOne->SetLeftAnimation(std::make_shared<Animation>(m_renderer.LoadTexture("assets/player_left.png"), 3, 28, 28, 150, true));
    m_playerOne->SetUpAnimation(std::make_shared<Animation>(m_renderer.LoadTexture("assets/player_up.png"), 3, 28, 28, 150, true));
    m_playerOne->SetDownAnimation(std::make_shared<Animation>(m_renderer.LoadTexture("assets/player_down.png"), 3, 28, 28, 150, true));

    Reset();

    m_renderer.PlayMusic(m_easyMusic);
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
        m_mainMenuAnimation.Update();

        if (m_modeSwitch && m_state.input.select)
            return;

        m_modeSwitch = false;

        // Change the selected button index
        if (m_state.input.down)
        {
            if (!m_menuButtonDownLast)
            {
                m_renderer.PlaySFX(m_clickSound);

                m_mainMenuSelectedButtonIndex = m_mainMenuSelectedButtonIndex == 3 ? 0 : m_mainMenuSelectedButtonIndex + 1;
                m_menuButtonDownLast = true;
            }
        }
        else if (m_state.input.up)
        {
            if (!m_menuButtonDownLast)
            {
                m_renderer.PlaySFX(m_clickSound);

                m_mainMenuSelectedButtonIndex = m_mainMenuSelectedButtonIndex == 0 ? 3 : m_mainMenuSelectedButtonIndex - 1;
                m_menuButtonDownLast = true;
            }
        }
        else
        {
            m_menuButtonDownLast = false;
        }

        // Reset all buttons
        m_buttonEasyAnimation.SetFrame(0);
        m_buttonNormalAnimation.SetFrame(0);
        m_buttonInsaneAnimation.SetFrame(0);
        m_buttonExitAnimation.SetFrame(0);

        if (m_mainMenuSelectedButtonIndex == 0)
        {
            m_buttonEasyAnimation.SetFrame(1);
        }
        else if (m_mainMenuSelectedButtonIndex == 1)
        {
            m_buttonNormalAnimation.SetFrame(1);
        }
        else if (m_mainMenuSelectedButtonIndex == 2)
        {
            m_buttonInsaneAnimation.SetFrame(1);
        }
        else if (m_mainMenuSelectedButtonIndex == 3)
        {
            m_buttonExitAnimation.SetFrame(1);
        }

        if (m_state.input.select)
        {
            m_renderer.PlaySFX(m_selectSound);

            // Button selected, handle it
            if (m_mainMenuSelectedButtonIndex == 0)
            {
                // Play easy
                m_selectedDifficulty = {
                    GameDifficulty::EASY,
                    EASY_STARTING_RATE,
                    EASY_MOVE_SPEED,
                    EASY_RATE_INCREASE,
                    EASY_RATE_MIN
                };

                Reset();
                m_state.status = GameStatus::RUNNING;

                m_renderer.PlayMusic(m_easyMusic);
            }
            else if (m_mainMenuSelectedButtonIndex == 1)
            {
                // Play normal
                m_selectedDifficulty = {
                    GameDifficulty::NORMAL,
                    NORMAL_STARTING_RATE,
                    NORMAL_MOVE_SPEED,
                    NORMAL_RATE_INCREASE,
                    NORMAL_RATE_MIN
                };

                Reset();
                m_state.status = GameStatus::RUNNING;

                m_renderer.PlayMusic(m_mediumMusic);
            }
            else if (m_mainMenuSelectedButtonIndex == 2)
            {
                // Play insane
                m_selectedDifficulty = {
                    GameDifficulty::INSANE,
                    INSANE_STARTING_RATE,
                    INSANE_MOVE_SPEED,
                    INSANE_RATE_INCREASE,
                    INSANE_RATE_MIN
                };                
                
                Reset();
                m_state.status = GameStatus::RUNNING;

                m_renderer.PlayMusic(m_insaneMusic);
            }
            else if (m_mainMenuSelectedButtonIndex == 3)
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
            if (!m_menuButtonDownLast)
            {
                m_renderer.PlaySFX(m_clickSound);

                m_gameoverMenuSelectedButtonIndex = m_gameoverMenuSelectedButtonIndex == 1 ? 0 : m_gameoverMenuSelectedButtonIndex + 1;
                m_menuButtonDownLast = true;
            }
        }
        else if (m_state.input.up)
        {
            if (!m_menuButtonDownLast)
            {
                m_renderer.PlaySFX(m_clickSound);

                m_gameoverMenuSelectedButtonIndex = m_gameoverMenuSelectedButtonIndex == 0 ? 1 : m_gameoverMenuSelectedButtonIndex - 1;
                m_menuButtonDownLast = true;
            }
        }
        else
        {
            m_menuButtonDownLast = false;
        }

        // Reset all buttons
        m_buttonRetryAnimation.SetFrame(0);
        m_buttonExitLargeAnimation.SetFrame(0);

        if (m_gameoverMenuSelectedButtonIndex == 0)
        {
            m_buttonRetryAnimation.SetFrame(1);
        }
        else if (m_gameoverMenuSelectedButtonIndex == 1)
        {
            m_buttonExitLargeAnimation.SetFrame(1);
        }

        if (m_state.input.select)
        {
            // Button selected, handle it
            m_renderer.PlaySFX(m_selectSound);
            if (m_gameoverMenuSelectedButtonIndex == 0)
            {
                // Handle retry
                m_state.status = GameStatus::RUNNING;
            }
            else if (m_gameoverMenuSelectedButtonIndex == 1)
            {
                // Handle exit
                m_state.status = GameStatus::MENU;
                m_modeSwitch = true;

                m_renderer.PlayMusic(m_easyMusic);
            }
            Reset();
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

            e = std::make_shared<Enemy>(p, m_selectedDifficulty.moveSpeed, CreateVirusAnimation(m_enemyTexture));
            e->direction = dir;

            m_enemies.push_back(e);

            // Update rate as level increases
            if (m_state.score > 0 && m_state.score % 5 == 0)
            {
                if (m_selectedDifficulty.startingRate >= m_selectedDifficulty.rateTimeoutMin + m_selectedDifficulty.rateIncrease)
                {
                    m_selectedDifficulty.startingRate -= m_selectedDifficulty.rateIncrease;
                }
                else
                {
                    m_selectedDifficulty.startingRate = m_selectedDifficulty.rateTimeoutMin;
                }
                
                m_fireTimer.SetTimeout(m_selectedDifficulty.startingRate);

                SDL_Log(std::to_string(m_selectedDifficulty.startingRate).c_str());
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
                    m_renderer.PlaySFX(m_positiveSound);

                    m_state.score++;

                    if (!m_highScoreFlag && m_state.bestScore != 0 && m_state.score > m_state.bestScore)
                    {
                        m_renderer.PlaySFX(m_highScoreSound);
                        m_highScoreFlag = true;
                    }
                }
                else
                {
                    m_renderer.PlaySFX(m_negativeSound);

                    m_state.status = GameStatus::GAME_OVER;

                    if (m_state.score > m_state.bestScore)
                    {
                        // Update best score
                        m_state.bestScore = m_state.score;

                        m_DB.AddScore(static_cast<int>(m_selectedDifficulty.difficulty), m_state.score);
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
        // Render title
        Rectangle logoRect = {18 * TEXTURE_SCALE * 4, 57 * TEXTURE_SCALE * 4 + 120, 180 * 3 - 20, 100 * 2};
        m_renderer.RenderWholeTexture(m_titleLogo, logoRect);

        m_mainMenuAnimation.Render(m_renderer, 590, 585);

        // Render buttons
        m_buttonEasyAnimation.Render(m_renderer, 18 * TEXTURE_SCALE * 4, 57 * TEXTURE_SCALE * 4);
        m_buttonNormalAnimation.Render(m_renderer, 18 * TEXTURE_SCALE * 4, 42 * TEXTURE_SCALE * 4);
        m_buttonInsaneAnimation.Render(m_renderer, 18 * TEXTURE_SCALE * 4, 27 * TEXTURE_SCALE * 4);
        m_buttonExitAnimation.Render(m_renderer, 35 * TEXTURE_SCALE * 4, 10 * TEXTURE_SCALE * 4);

        // Render high scores
        Rectangle easyRect = {68 * TEXTURE_SCALE * 4, 57 * TEXTURE_SCALE * 4, 15 * TEXTURE_SCALE * 4, 12 * TEXTURE_SCALE * 4};
        Rectangle easyNumRect = easyRect;
        easyNumRect.pos.x += 24;
        easyNumRect.size.w -= 40;
        easyNumRect.size.h += 16;
        m_renderer.RenderWholeTexture(m_scoreFrameTexture, easyRect);
        m_renderer.RenderFont(m_UIFont, std::to_string(m_state.bestScoreEasy), easyNumRect, FG_COLOR);

        Rectangle normalRect = {68 * TEXTURE_SCALE * 4, 42 * TEXTURE_SCALE * 4, 15 * TEXTURE_SCALE * 4, 12 * TEXTURE_SCALE * 4};
        Rectangle normalNumRect = normalRect;
        normalNumRect.pos.x += 24;
        normalNumRect.size.w -= 40;
        normalNumRect.size.h += 16;
        m_renderer.RenderWholeTexture(m_scoreFrameTexture, normalRect);
        m_renderer.RenderFont(m_UIFont, std::to_string(m_state.bestScoreNormal), normalNumRect, FG_COLOR);

        Rectangle insaneRect = {68 * TEXTURE_SCALE * 4, 27 * TEXTURE_SCALE * 4, 15 * TEXTURE_SCALE * 4, 12 * TEXTURE_SCALE * 4};
        Rectangle insaneNumRect = insaneRect;
        insaneNumRect.pos.x += 24;
        insaneNumRect.size.w -= 40;
        insaneNumRect.size.h += 16;
        m_renderer.RenderWholeTexture(m_scoreFrameTexture, insaneRect);
        m_renderer.RenderFont(m_UIFont, std::to_string(m_state.bestScoreInsane), insaneNumRect, FG_COLOR);
    }
    else if (m_state.status == GameStatus::GAME_OVER)
    {
        m_renderer.RenderWholeTexture(m_overlayTexture, {0, 0, 800, 800});

        // Render score
        float w = 160;
        float h = 40;
        float offset = 10;
        Rectangle scoreRect = {offset, WORLDSIZE_H - h - offset, w, h};
        m_renderer.RenderFont(m_UIFont, "SCORE: " + std::to_string(m_state.score), scoreRect, { 0x2F, 0x38, 0x0F, 0xFF });

        Rectangle bestScoreRect = {WORLDSIZE_W - w - offset, WORLDSIZE_H - h - offset, w, h};
        m_renderer.RenderFont(m_UIFont, "BEST: " + std::to_string(m_state.bestScore), bestScoreRect, { 0x2F, 0x38, 0x0F, 0xFF });

        // Render buttons

        m_buttonRetryAnimation.Render(m_renderer, 400 - 188, 400 + 48);
        m_buttonExitLargeAnimation.Render(m_renderer, 400 - 188, 400 - 44 - 40);
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
        float w = 160;
        float h = 40;
        float offset = 10;
        Rectangle scoreRect = {offset, WORLDSIZE_H - h - offset, w, h};
        m_renderer.RenderFont(m_UIFont, "SCORE: " + std::to_string(m_state.score), scoreRect, { 0x2F, 0x38, 0x0F, 0xFF });

        Rectangle bestScoreRect = {WORLDSIZE_W - w - offset, WORLDSIZE_H - h - offset, w, h};
        m_renderer.RenderFont(m_UIFont, "BEST: " + std::to_string(m_state.bestScore), bestScoreRect, { 0x2F, 0x38, 0x0F, 0xFF });
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
    m_highScoreFlag = false;

    m_state.score = 0;
    m_state.bestScore = m_DB.GetHighScore(static_cast<int>(m_selectedDifficulty.difficulty));

    if (m_selectedDifficulty.difficulty == GameDifficulty::EASY)
    {
        m_selectedDifficulty.startingRate = EASY_STARTING_RATE;
    }
    else if (m_selectedDifficulty.difficulty == GameDifficulty::NORMAL)
    {
        m_selectedDifficulty.startingRate = NORMAL_STARTING_RATE;
    }
    if (m_selectedDifficulty.difficulty == GameDifficulty::INSANE)
    {
        m_selectedDifficulty.startingRate = INSANE_STARTING_RATE;
    }

    m_state.bestScoreEasy = m_DB.GetHighScore(static_cast<int>(GameDifficulty::EASY));
    m_state.bestScoreNormal = m_DB.GetHighScore(static_cast<int>(GameDifficulty::NORMAL));
    m_state.bestScoreInsane = m_DB.GetHighScore(static_cast<int>(GameDifficulty::INSANE));

    m_fireTimer.SetTimeout(m_selectedDifficulty.startingRate);
    m_fireTimer.Reset();

    m_enemies.clear();
}