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


	SoundSource* bgm = NewGO<SoundSource>(bgmID);
	bgm->Init(bgmID);
	bgm->Play(false);
}