#include "GameState.h"

GameState::GameState()
{
}

UserInputState &GameState::GetInputState()
{
    return m_inputState;
}

void GameState::SetPlayerPosition(Point position)
{
    m_playerPosition = position;
}

Point GameState::GetPlayerPosition()
{
    return m_playerPosition;
}