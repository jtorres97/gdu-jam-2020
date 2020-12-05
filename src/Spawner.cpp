#include "Spawner.h"

Spawner::Spawner(Point position) : m_spawnTimer(1000), Entity()
{
    m_position = position;
    m_bound = { 128, 128 };

    m_spawnTimer.Reset();
}

void Spawner::Update(GameState &state)
{
    // Update enemies under spawner control
    for (auto e : m_enemies)
    {
        e->Update(state);
    }

    // Spawn additional enemies
    if (m_spawnTimer.IsExpired())
    {
        Point enemyPosition = m_position + Point{0, -100};
        auto ne = std::make_shared<Enemy>(enemyPosition);
        ne->SetMainTexture(m_enemyTexture);
        m_enemies.push_back(ne);
        m_spawnTimer.Reset();
    }
}

void Spawner::Render(SDLRenderer &renderer)
{
    renderer.RenderWholeTexture(m_mainTexture, GetHitBox());

    for (auto e : m_enemies)
    {
        e->Render(renderer);
    }
}

void Spawner::SetMainTexture(std::shared_ptr<Texture> mainTexture)
{
    m_mainTexture = mainTexture;
}

void Spawner::SetEnemyTexture(std::shared_ptr<Texture> enemyTexture)
{
    m_enemyTexture = enemyTexture;
}