#include "Player.h"

Player::Player() : m_flipTimer(250), Entity()
{
    m_bound = {8 * TEXTURE_SCALE, 8 * TEXTURE_SCALE};
    m_position = {WORLDSIZE_W / 2, WORLDSIZE_H / 2};

    m_flipTimer.Reset();

    m_maxHP = 100;
    m_HP = m_maxHP;
}

void Player::Update(GameState &state)
{
    auto input = state.GetInputState();
    Vector2D keyVector = WASDToMovementVector(input.up, input.down, input.left, input.right);
    if (keyVector.GetMagnitude() > 0.001) 
    {
        m_rotation = keyVector.GetAngle();
    }
    // m_rotation = Vector2D(input.cursor - m_position).GetAngle();
}

void Player::Render(SDLRenderer &renderer)
{
    renderer.RenderWholeTextureRotate(m_mainTexture, GetHitBox(), m_rotation * TO_DEGS, { m_bound.w / 2, m_bound.h / 2 });
}

void Player::SetMainTexture(std::shared_ptr<Texture> mainTexture)
{
    m_mainTexture = mainTexture;
}

float Player::GetRotation()
{
    return m_rotation;
}

bool Player::GetFlip()
{
    return m_flip;
}