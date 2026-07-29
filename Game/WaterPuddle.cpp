#include "stdafx.h"
#include "WaterPuddle.h"
#include "Player.h"

WaterPuddle::WaterPuddle()
{
}

WaterPuddle::~WaterPuddle()
{
}

bool WaterPuddle::Start()
{
    m_model.Init("Assets/modelData/water2.tkm");

    m_player = FindGO<Player>("player");

    return true;
}

void WaterPuddle::Update()
{
    m_model.SetPosition(m_position);
    m_model.Update();

    if (m_player == nullptr)
    {
        return;
    }

    Vector3 diff = m_player->GetPosition() - m_position;
    diff.y = 0.0f;

    bool inside = (diff.Length() < m_radius);

    if (inside && !m_playerInside)
    {
        m_player->SetMoveSpeedRate(0.5f);
    }

    m_playerInside = inside;
}

void WaterPuddle::Render(RenderContext& rc)
{
    m_model.Draw(rc);
}

void WaterPuddle::SetPosition(const Vector3& pos)
{
    m_position = pos;
}

