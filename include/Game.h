#pragma once

#include <memory>
#include <chrono>
#include <vector>

#include "Renderer.h"
#include "GameState.h"
#include "Entity.h"

const double TICK_RATE = 60;
const auto TIME_PER_TICK = std::chrono::duration<double>(1.0 / TICK_RATE);

class Game
{
public:
    void Initialize();
    void Run();
    void Close();

private:
    bool m_isRunning;
    GameState m_state;
    SDLRenderer m_renderer;
    std::vector<std::shared_ptr<Entity>> m_entities;
};