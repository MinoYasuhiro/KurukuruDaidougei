#pragma once

//サウンドオプション画面の遷移元を判別する目印
enum TransitionSource
{
	FromPause,	//ポーズ画面から
	FromTitle,	//タイトル画面から
};

class Game;
class SoundUI;
class SoundOption :public IGameObject
{
public:
	SoundOption() {}
	~SoundOption() {}
	bool Start();
	void Update();
	void Render(RenderContext& renderContext);

	//遷移元を外部から設定するための関数
	void SetTransitionSource(TransitionSource transitionSource);

private:
	//サウンドオプション画面の背景スプライト
	SpriteRender m_spriteRender;

	//サウンドオプション画面の文字スプライト
	SpriteRender m_textRender;

	//オプション画面上での操作説明
	SpriteRender m_operationInstructions;

	//表示位置
	Vector3 m_position;

	//サウンド設定用UI
	SoundUI* m_soundUI;

	//開かれたときの遷移元を記憶する変数
	TransitionSource m_transitionSource;
};

