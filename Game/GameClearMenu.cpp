#include "stdafx.h"
#include "GameClearMenu.h"

namespace {

    const Vector3 RETRY_POSITION = { -600.0f, -200.0f, 0.0f };
    const Vector3 BACKTITLE_POSITION = { 600.0f, -200.0f, 0.0f };
    

}
bool GameClearMenu::Start()
{
    m_retryRender.Init("Assets/Sprite/Retry.DDS", 500, 300);
    m_titleRender.Init("Assets/sprite/BackTitle.dds", 500.0f, 300.0f);

    m_retryRender.SetPosition(Vector3(RETRY_POSITION));
    m_titleRender.SetPosition(Vector3(BACKTITLE_POSITION));

    return true;
}

void GameClearMenu::Update()
{
    Input();
    SelectScale();

    m_retryRender.Update();
    m_titleRender.Update();
}

void GameClearMenu::Input()
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
            m_result = ClearMenuResult::Retry;
        else
            m_result = ClearMenuResult::BTitle;
    }
}


void GameClearMenu::SelectScale()
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

void GameClearMenu::Render(RenderContext& rc)
{
    m_retryRender.Draw(rc);
    m_titleRender.Draw(rc);
}

