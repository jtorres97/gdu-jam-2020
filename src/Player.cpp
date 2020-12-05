#include "Player.h"

Player::Player() : m_flipTimer(250), Entity()
{
    m_position = { 100, 100 };
    m_bound = { 64, 64 };

    m_flipTimer.Reset();
}

void Player::Update(GameState &state)
{
    auto input = state.GetInputState();

    // Movement
    auto moveVector = WASDToMovementVector(input.up, input.down, input.left, input.right);

    // TODO: handle acceleration

    m_velocity = moveVector.Scale(m_moveSpeed);
    m_position = m_position + m_velocity.GetPoint();

    // Angle
    m_rotation = Vector2D(input.cursor - m_position).GetAngle();

    // Flip
    if (input.fireMain && m_flipTimer.IsExpired()) 
    {
        m_flip = !m_flip;
        m_flipTimer.Reset();
    }

    // Update player pos in the game state
    state.SetPlayerPosition(m_position);
}

void Player::Render(SDLRenderer &renderer)
{
    float angle = m_flip ? m_rotation + Pi : m_rotation;
    renderer.RenderWholeTextureRotate(m_mainTexture, GetHitBox(), angle * TO_DEGS, { m_bound.w / 2, m_bound.h / 2 });
}

void Player::SetMainTexture(std::shared_ptr<Texture> mainTexture)
{
    m_mainTexture = mainTexture;
}