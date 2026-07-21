#include "stdafx.h"
#include "SEManager.h"
#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"
#include "SoundSettings.h"
#include <unordered_map>	//キーと値のセットで管理する箱

//再生中のSEを管理するキャッシュ
//キャッシュ→作ったものを覚えておく仕組み
static std::unordered_map<SE, SoundSource*>g_seSources;

void SEManager::Init()
{
	//効果音ファイルをサウンドエンジンに登録
	g_soundEngine->ResistWaveFileBank(SE_umbrellaSpinning, "Assets/sound/umbrellaSpinning.wav");
	g_soundEngine->ResistWaveFileBank(SE_run, "Assets/sound/run.wav");
	g_soundEngine->ResistWaveFileBank(SE_ball, "Assets/sound/ball.wav");
	g_soundEngine->ResistWaveFileBank(SE_teaBowl, "Assets/sound/teaBowl.wav");
	g_soundEngine->ResistWaveFileBank(SE_masu, "Assets/sound/masu.wav");
	g_soundEngine->ResistWaveFileBank(SE_penguin, "Assets/sound/penguinLntimidation.wav");
	g_soundEngine->ResistWaveFileBank(SE_bone, "Assets/sound/bone.wav");
	g_soundEngine->ResistWaveFileBank(SE_spotlight, "Assets/sound/spotlight.wav");
	g_soundEngine->ResistWaveFileBank(SE_cheers, "Assets/sound/cheers.wav");
	g_soundEngine->ResistWaveFileBank(SE_crackedEgg, "Assets/sound/crackedEgg.wav");
	g_soundEngine->ResistWaveFileBank(SE_booing, "Assets/sound/booing.wav");
	g_soundEngine->ResistWaveFileBank(SE_hitHead, "Assets/sound/hitHead.wav");
	g_soundEngine->ResistWaveFileBank(SE_smartphone, "Assets/sound/smartphone.wav");
	g_soundEngine->ResistWaveFileBank(SE_buttonInputSuccess, "Assets/sound/QTEButton.wav");
	g_soundEngine->ResistWaveFileBank(SE_cursor, "Assets/sound/ka-soru.wav");
	g_soundEngine->ResistWaveFileBank(SE_pause, "Assets/sound/Pause.wav");
	g_soundEngine->ResistWaveFileBank(SE_choice, "Assets/sound/Choice.wav");
	g_soundEngine->ResistWaveFileBank(SE_return, "Assets/sound/Return.wav");
	g_soundEngine->ResistWaveFileBank(SE_decision, "Assets/sound/decision.wav");
	g_soundEngine->ResistWaveFileBank(SE_QTEmiss, "Assets/sound/QTEmiss.wav");
}

void SEManager::Play(SE seID, bool loop)
{
	if (loop)
	{
		//すでに同じSEが再生中なら一度削除
	//同じ音を重複管理しないため
		auto it = g_seSources.find(seID);
		if (it != g_seSources.end())
		{
			if (it->second != nullptr && (uintptr_t)it->second != 0xFFFFFFFFFFFFFFFF)
			{
				it->second->Stop();
				DeleteGO(it->second);
			}
			g_seSources.erase(it);
		}
		//効果音用のサウンドソースを生成
		SoundSource*se = NewGO<SoundSource>(seID);

		//登録済みのSEを初期化
		se->Init(seID);

		//音量設定(マスター音量×SE音量）
		se->SetVolume(SoundSettings::Master * SoundSettings::SE);

		//効果音再生(loop=trueでループ)
		se->Play(true);

		//キャッシュに保存(後で停止や管理に使う)
		g_seSources[seID] = se;
	}
	else
	{
		SoundSource* se = NewGO<SoundSource>(seID);
		se->Init(seID);
		se->SetVolume(SoundSettings::Master * SoundSettings::SE);
		se->Play(false);
	}
}

//キャッシュ全削除
void SEManager::ClearCache()
{
	for (auto& pair : g_seSources)
	{
		if (pair.second != nullptr && (uintptr_t)pair.second != 0xFFFFFFFFFFFFFFFF)
		{
			pair.second->Stop();
			//再生中のSEを削除
			DeleteGO(pair.second);
		}
	}
	//マップを空にする
	g_seSources.clear();
}

void SEManager::StopLoop(SE seID)
{
	auto it = g_seSources.find(seID);

	//見つからなければ何もしない
	if (it == g_seSources.end())return;

	//サウンド削除→再生停止
	DeleteGO(it->second);

	//キャッシュからも削除
	g_seSources.erase(it);
}