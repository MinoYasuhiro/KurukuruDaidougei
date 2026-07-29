#include "stdafx.h"
#include "BGMManager.h"
#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"
#include "SoundSettings.h"

void BGMManager::Init()
{
	g_soundEngine->ResistWaveFileBank(BGM_Title, "Assets/BGM/Title.wav");
	g_soundEngine->ResistWaveFileBank(BGM_NormalUmbrella, "Assets/BGM/Normal_umbrella_BGM.wav");
	g_soundEngine->ResistWaveFileBank(BGM_AudienceParticipationUmbrella, "Assets/BGM/Audience-participation_umbrella_BGM.wav");
	g_soundEngine->ResistWaveFileBank(BGM_GameClear, "Assets/BGM/GmeClear.wav");
    g_soundEngine->ResistWaveFileBank(BGM_GameOver, "Assets/BGM/GameOver.wav");
}



void BGMManager::Play(BGM bgmID)
{
    //IDが一致しているなら再生しないようにする
    if (m_currentBGMId == bgmID)
    {
        return;
    }

    Stop();

    m_currentBGM = NewGO<SoundSource>(0);
    m_currentBGM->Init(bgmID);

    m_currentBGM->SetVolume(SoundSettings::Master * SoundSettings::BGM);

    m_currentBGM->Play(true); // ループ

    //再生中のBGMIDを保持
    m_currentBGMId = bgmID;
}

void BGMManager::Stop()
{
    if (m_currentBGM)
    {
        m_currentBGM->Stop();
        DeleteGO(m_currentBGM);
        m_currentBGM = nullptr;
    }
    //BGMが完全に止まったのでIDリセット
    m_currentBGMId = -1;
}

void BGMManager::ApplyVolume()
{
    if (!m_currentBGM)return;

    m_currentBGM->SetVolume(SoundSettings::Master * SoundSettings::BGM);
}

