#pragma once

#include <memory>
#include <chrono>

#include "Renderer.h"
#include "UserInput.h"

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
    UserInputState m_inputState;
    SDLRenderer m_renderer;
};