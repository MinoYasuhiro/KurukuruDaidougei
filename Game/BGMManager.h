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

private:
	SoundSource* m_currentBGM = nullptr;
};

