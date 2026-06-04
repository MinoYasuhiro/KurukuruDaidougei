//後々追加
#pragma once
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
};

