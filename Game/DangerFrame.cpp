
#include "stdafx.h"
#include "DangerFrame.h"

bool DangerFrame::Start()
{
    m_sprite.Init("Assets/effect/DangerFreame.DDS", 1920.0f, 1080.0f);

    m_sprite.SetPosition({ 0.0f, 0.0f, 0.0f });

    return true;
}

void DangerFrame::Update()
{
    if (m_player == nullptr)
    {
        m_player = FindGO<Player>("player");
    }

    if (m_player == nullptr)
    {
        return;
    }

    // 2回失敗した瞬間
    if (m_player->m_playerError >= 2 &&
        !m_isStarted)
    {
        m_isStarted = true;
        m_delayTimer = 3.0f;
    }

    // カウントダウン
    if (m_isStarted && !m_isDraw)
    {
        m_delayTimer -= g_gameTime->GetFrameDeltaTime();

        if (m_delayTimer <= 0.0f)
        {
            m_isDraw = true;
        }
    }

    // 脈動
    if (m_isDraw)
    {
        m_timer += g_gameTime->GetFrameDeltaTime();

        float alpha = 0.6f + sinf(m_timer * 3.0f) * 0.2f;

        m_sprite.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
    }

    m_sprite.Update();
}

void DangerFrame::Render(RenderContext& rc)
{
    if (!m_isDraw)
    {
        return;
    }

    m_sprite.Draw(rc);
}

void DangerFrame::Reset()
{
    m_isDraw = false;

    m_isStarted = false;

    m_delayTimer = 0.0f;

    m_timer = 0.0f;

    m_sprite.SetMulColor(Vector4( 1.0f, 1.0f, 1.0f, 1.0f ));
}