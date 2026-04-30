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
	static void Play(SE seID,bool loop);

	static void StopLoop(SE seID);

	static void ClearCache();
};

