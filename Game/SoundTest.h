#pragma once
#include "SoundUI.h"
#include "BGMManager.h"

class SoundTest : public IGameObject
{
public:
	SoundTest() {}
	~SoundTest() {}

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

private:
	SpriteRender m_background;
	SpriteRender m_backgroundTitle;
	BGMManager* m_bgmManager = nullptr;
	SoundUI* m_soundUI = nullptr;   
};
