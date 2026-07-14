#pragma once

class SEManager;
class SoundUI :public IGameObject
{
public:
	SoundUI();
	~SoundUI();
	bool Start()override;
	void Update()override;
	void Render(RenderContext& renderContext)override;

	//入力処理
	void Input();

	//バーの表示位置を音量地に応じて行進
	void FillBars();

	bool IsFinished()const;

	//外部からSoundSource(BGM/SE)を設定
	void SetSoundSources(SoundSource* bgm, SoundSource* se)
	{
		m_BGMSource = bgm;
		m_SESource = se;
	}

	//現在の設定値を実際の音量に反映
	//void ApplyVolumes();

	//選択中のバーを拡大表示
	void SelectScale();
private:
	//各選択肢
	SpriteRender m_masterRender;
	SpriteRender m_SERender;
	SpriteRender m_BGMRender;

	//各音量バー
	SpriteRender m_masterBarRender;
	SpriteRender m_SEBarRender;
	SpriteRender m_BGMBarRender;

	//各音量バーのメモリ
	SpriteRender m_mastarFillRender;
	SpriteRender m_SEFillRender;
	SpriteRender m_BGMFillRender;

	SpriteRender m_masterFillSelection;
	SpriteRender m_SEFillSelection;
	SpriteRender m_BGMFillSelection;

	static const int kSeparatorNum = 11;
	SpriteRender m_separator[3][kSeparatorNum];

	//音源(外部から注入される)
	SoundSource* m_BGMSource = nullptr;
	SoundSource* m_SESource = nullptr;

	//現在選択されている項目
	int m_selectedIndex = 0;

	//編集モード中かどうか
	bool m_isEditing = false;

	//UI終了フラグ
	bool m_isFinished = false;

	//各音量の現在地
	float m_masterValue = 0.5f;
	float m_SEValue = 0.5f;
	float m_BGMValue = 0.5f;

	//音量バーの移動範囲
	const float m_minX = -500.0f;
	const float m_maxX = 500.0f;

	//入力連打防止用タイマー
	float m_inputTimer = 0.0f;
	const float m_inputInterval = 0.15f;
};