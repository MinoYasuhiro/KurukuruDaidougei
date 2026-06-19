#include "stdafx.h"
#include "GameOverMenu.h"

namespace {
    const Vector3 RETRY_POSITION = { -600.0f, -200.0f, 0.0f };
    const Vector3 BACKTITLE_POSITION = { 600.0f, -200.0f, 0.0f };
}

bool GameOverMenu::Start()
{
    m_retryRender.Init("Assets/Sprite/Retry.DDS", 500, 300);
    m_titleRender.Init("Assets/Sprite/BackTitle.DDS", 500, 300);

    m_retryRender.SetPosition(RETRY_POSITION);
    m_titleRender.SetPosition(BACKTITLE_POSITION);

    m_selectIndex = 0;
    SelectScale();

    return true;
}

void GameOverMenu::Update()
{
    Input();
    SelectScale();

    m_retryRender.Update();
    m_titleRender.Update();
}

void GameOverMenu::Input()
{
    m_inputTimer -= 1.0f / 60.0f;

    if (m_inputTimer > 0.0f)
        return;

    if (g_pad[0]->IsPress(enButtonLeft))
    {
        m_selectIndex--;
        m_inputTimer = m_inputInterval;
    }

    if (g_pad[0]->IsPress(enButtonRight))
    {
        m_selectIndex++;
        m_inputTimer = m_inputInterval;
    }

    // ループ
    if (m_selectIndex < 0) m_selectIndex = 1;
    if (m_selectIndex > 1) m_selectIndex = 0;

    // 決定
    if (g_pad[0]->IsPress(enButtonA))
    {
        if (m_selectIndex == 0)
            m_result = OverMenuResult::Retry;
        else
            m_result = OverMenuResult::Btitle;
    }
}

void GameOverMenu::SelectScale()
{
    if (m_selectIndex == 0)
    {
        m_retryRender.SetScale({ 1.8f,1.8f,1.8f });
        m_titleRender.SetScale({ 1.5f,1.5f,1.5f });
    }
    else
    {
        m_retryRender.SetScale({ 1.5f,1.5f,1.5f });
        m_titleRender.SetScale({ 1.8f,1.8f,1.8f });
    }
}

void GameOverMenu::Render(RenderContext& rc)
{
    m_retryRender.Draw(rc);
    m_titleRender.Draw(rc);
}