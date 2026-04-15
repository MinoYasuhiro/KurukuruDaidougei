#pragma once
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
	BGMManager* m_bgmManager = nullptr;
};
