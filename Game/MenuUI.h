#pragma once

enum class MenuResult
{
	None,		//何も選択されていない
	BackTitle,	//タイトルへ戻る
	Restart,	//ゲーム再開
	Retry,		//始めから
	SoundOption,//サウンド設定
};

class MenuUI :public IGameObject
{
public:
	MenuUI();
	~MenuUI();
	bool Start()override;
	void Update()override;
	void Render(RenderContext& renderContext)override;

	void Input();		//入力処理（十字キー+決定）
	void SelectScale();	//選択中UIの拡大処理

	//外部から結果を取得する
	MenuResult GetResult()const { return m_result; }

private:
	//各メニュー項目のスプライト
	SpriteRender m_backTitleRender;
	SpriteRender m_retryRender;
	SpriteRender m_restartRender;
	SpriteRender m_soundTestRender;

	MenuResult m_result = MenuResult::None;//現在の選択結果

	int m_selectIndex = 0;//現在選択中のインデックス(0～3)
};
