#include "Player.h"

Player::Player() : Entity()
{
    m_bound = {28 * TEXTURE_SCALE, 28 * TEXTURE_SCALE};
    m_position = {WORLDSIZE_W / 2, WORLDSIZE_H / 2};
}

void Player::Update(GameState &state)
{
    auto input = state.input;
    Vector2D keyVector = WASDToMovementVector(input.up, input.down, input.left, input.right);
    if (keyVector.GetMagnitude() > 0.001)
    {
        m_rotation = keyVector.GetAngle();
    }
    // m_rotation = Vector2D(input.cursor - m_position).GetAngle();

    m_rightAnimation->Update();
    m_leftAnimation->Update();
    m_upAnimation->Update();
    m_downAnimation->Update();
}

void Player::Render(SDLRenderer &renderer)
{
    if (CloseEnough(m_rotation, 0.0))
    {
        m_rightAnimation->Render(renderer, m_position.x - m_bound.w / 2, m_position.y - m_bound.h / 2);
    }
    else if (CloseEnough(m_rotation, Pi / 2))
    {
        m_upAnimation->Render(renderer, m_position.x - m_bound.w / 2, m_position.y - m_bound.h / 2);
    }
    else if (CloseEnough(m_rotation, Pi))
    {
        m_leftAnimation->Render(renderer, m_position.x - m_bound.w / 2, m_position.y - m_bound.h / 2);
    }
    else if (CloseEnough(m_rotation, -Pi / 2))
    {
        m_downAnimation->Render(renderer, m_position.x - m_bound.w / 2, m_position.y - m_bound.h / 2);
    }
}

void Player::SetRightAnimation(std::shared_ptr<Animation> animation)
{
    m_rightAnimation = animation;
}

void Player::SetLeftAnimation(std::shared_ptr<Animation> animation)
{
    m_leftAnimation = animation;
}

void Player::SetUpAnimation(std::shared_ptr<Animation> animation)
{
    m_upAnimation = animation;
}

void Player::SetDownAnimation(std::shared_ptr<Animation> animation)
{
    m_downAnimation = animation;
}

float Player::GetRotation()
{
    return m_rotation;
}