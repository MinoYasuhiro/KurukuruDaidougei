#pragma once
#include "BGM.h"

class BGMManager : public IGameObject
{
public:
	BGMManager() {}
	~BGMManager() {}
	void Init();
	void Play(BGM bgmID);
	void Stop();
	void ApplyVolume();

private:
	SoundSource* m_currentBGM = nullptr;
	int m_currentBGMId = -1;	//現在のBGMIDを記憶する変数
};

