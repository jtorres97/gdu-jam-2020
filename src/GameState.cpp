#include "GameState.h"

GameState::GameState()
{
}

UserInputState& GameState::GetInputState()
{
    return m_inputState;
}