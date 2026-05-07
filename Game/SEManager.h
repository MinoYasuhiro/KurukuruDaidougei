#pragma once
#include "SE.h"

class SEManager
{
public:
	SEManager() {};
	~SEManager() {};

	//効果音の初期化
	static void Init();

	//指定した効果音を再生
	//seID: 再生する効果音ID
	//loop: ループ再生するか
	static void Play(SE seID, bool loop);

	//ループ再生中の効果音停止
	static void StopLoop(SE seID);

	//全ての効果音を削除(シーン切り替え時などに使用)
	static void ClearCache();
};

