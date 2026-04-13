#pragma once
class VolumeControl;
class SoundUI :public IGameObject
{
public:
	SoundUI();
	~SoundUI();
	bool Start()override;
	void Update()override;
	void Render(RenderContext& renderContext)override;
	void Input();
	void FillBars();
	void SetSoundSources(SoundSource* bgm, SoundSource* se)
	{
		m_BGMSource = bgm;
		m_SESource = se;
	}
	void ApplyVolumes();
	void SelectScale();
private:
	SpriteRender m_masterBarRender;
	SpriteRender m_SEBarRender;
	SpriteRender m_BGMBarRender;
	SpriteRender m_mastarFillRender;
	SpriteRender m_SEFillRender;
	SpriteRender m_BGMFillRender;
	SoundSource* m_BGMSource = nullptr;
	SoundSource* m_SESource = nullptr;

	int m_selectedIndex = 0;
	bool m_isEditing = false;
	float m_masterValue = 0.5f;
	float m_SEValue = 0.5f;
	float m_BGMValue = 0.5f;
	const float m_minX = -500.0f;
	const float m_maxX = 500.0f;
};