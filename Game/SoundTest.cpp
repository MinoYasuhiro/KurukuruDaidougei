#include "stdafx.h"
#include "SoundTest.h"
#include "Title.h"
#include "Game.h"

bool SoundTest::Start()
{
    // 背景
    m_background.Init("Assets/Sprite/SoundBackground.DDS", 1800, 1200);
    m_background.SetScale({ 1.5f, 1.5f, 1.5f });

    //サウンド文字
    m_backgroundTitle.Init("Assets/Sprite/Sound.DDS", 300, 300);
    m_backgroundTitle.SetPosition({ 0,1200,0 });


    // BGM管理取得
    m_bgmManager = FindGO<BGMManager>("bgmManager");
    if (m_bgmManager)
    {
        m_bgmManager->Play(BGM_AudienceParticipationUmbrella);
    }

    // ★ SoundUI を生成（前面）
    m_soundUI = NewGO<SoundUI>(200, "soundUI");


    // ★ 音源を UI に渡したい場合（後で使う）
    if (m_soundUI && m_bgmManager)
    {
        // m_soundUI->SetSoundSources(m_bgmManager->GetCurrentBGM(), nullptr);
    }

    return true;
}


void SoundTest::Update()
{
    m_background.Update();
    m_backgroundTitle.Update();

    if (g_pad[0]->IsPress(enButtonB) ||
        g_pad[0]->IsPress(enButtonX))
    {
        if (m_bgmManager)
            m_bgmManager->Stop();

        // ★ SoundUI を消す
        if (m_soundUI)
        {
            DeleteGO(m_soundUI);
            m_soundUI = nullptr;
        }

        if (Game* game = FindGO<Game>("game"))
            game->RequestTitle();

        DeleteGO(this);
    }
}

void SoundTest::Render(RenderContext& rc)
{
    m_background.Draw(rc);
    m_backgroundTitle.Draw(rc);
}
