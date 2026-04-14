#include "stdafx.h"
#include "SoundTest.h"
#include "Title.h"
#include "Game.h"

bool SoundTest::Start()
{
    // 背景
    m_background.Init("Assets/Sprite/Soundtest.DDS", 1800, 1200);
    m_background.SetPosition({ 0, 0, 1000 });
    m_background.SetScale({ 1.0f, 1.0f, 1.0f });

    // BGM管理取得
    m_bgmManager = FindGO<BGMManager>("bgmManager");
    if (m_bgmManager)
    {
        m_bgmManager->Play(BGM_AudienceParticipationUmbrella);
    }

    return true;
}


void SoundTest::Update()
{
    m_background.Update();

    // B or X で戻る
    if (g_pad[0]->IsTrigger(enButtonB) ||
        g_pad[0]->IsTrigger(enButtonX))
    {
        // BGM停止
        if (m_bgmManager)
        {
            m_bgmManager->Stop();
        }

        // ---- 戻り先 ----

        NewGO<Title>(0);
        DeleteGO(this);


    }
}


void SoundTest::Render(RenderContext& rc)
{
    m_background.Draw(rc);
}
