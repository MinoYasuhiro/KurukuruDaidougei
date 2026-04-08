#include "stdafx.h"
#include "SEManager.h"
#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"

void SEManager::Init()
{
	g_soundEngine->ResistWaveFileBank(SE_umbrellaSpinning, "Assets/sound/umbrellaSpinning.wav");
	g_soundEngine->ResistWaveFileBank(SE_run, "Assets/sound/run.wav");
	g_soundEngine->ResistWaveFileBank(SE_ball, "Assets/sound/ball.wav");
	g_soundEngine->ResistWaveFileBank(SE_teaBowl, "Assets/sound/teaBowl.wav");
	g_soundEngine->ResistWaveFileBank(SE_masu, "Assets/sound/masu.wav");
	g_soundEngine->ResistWaveFileBank(SE_penguin, "Assets/sound/penguin.wav");
	g_soundEngine->ResistWaveFileBank(SE_bone, "Assets/sound/bone.wav");
	g_soundEngine->ResistWaveFileBank(SE_spotlight, "Assets/sound/spotlight.wav");
	g_soundEngine->ResistWaveFileBank(SE_cheers, "Assets/sound/cheers.wav");
	g_soundEngine->ResistWaveFileBank(SE_crackedEgg, "Assets/sound/crackedEgg.wav");
	g_soundEngine->ResistWaveFileBank(SE_booing, "Assets/sound/booing.wav");
	g_soundEngine->ResistWaveFileBank(SE_hitHead, "Assets/sound/hitHead.wav");
	g_soundEngine->ResistWaveFileBank(SE_smartphone, "Assets/sound/smartphone.wav");
}

void SEManager::Play(SE seID)
{
	SoundSource* se = NewGO<SoundSource>(seID);
	se->Init(seID);
	se->Play(false);
}
