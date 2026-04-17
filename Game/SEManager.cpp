#include "stdafx.h"
#include "SEManager.h"
#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"

void SEManager::Init()
{
	//効果音ファイルをサウンドエンジンに登録
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
	//効果音用のサウンドソースを生成
	SoundSource* se = NewGO<SoundSource>(seID);

	//登録済みのSEを初期化
	se->Init(seID);

	//効果音を1回再生(ループなし)
	se->Play(false);
}
