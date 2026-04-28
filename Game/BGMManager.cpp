#include "stdafx.h"
#include "BGMManager.h"
#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"

void BGMManager::Init()
{
	g_soundEngine->ResistWaveFileBank(BGM_Title, "Assets/BGM/Title.wav");
	g_soundEngine->ResistWaveFileBank(BGM_NormalUmbrella, "Assets/BGM/Normal_umbrella_BGM.wav");
	g_soundEngine->ResistWaveFileBank(BGM_AudienceParticipationUmbrella, "Assets/BGM/Audience-participation_umbrella_BGM.wav");
	g_soundEngine->ResistWaveFileBank(BGM_GameClear, "Assets/BGM/GmeClear.wav");
}



void BGMManager::Play(BGM bgmID)
{
    //Stop();

    m_currentBGM = NewGO<SoundSource>(0);
    m_currentBGM->Init(bgmID);
    //m_currentBGM->Play(true); // ループ
}

//void BGMManager::Stop()
//{
//    if (m_currentBGM)
//    {
//        DeleteGO(m_currentBGM);
//        m_currentBGM = nullptr;
//    }
//}

