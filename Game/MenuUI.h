#pragma once

enum class MenuResult
{
	None,
	BackTitle,
	Restart,
	EndGame,
	SoundOption,
};

class MenuUI :public IGameObject
{
public:
	MenuUI();
	~MenuUI();
	bool Start()override;
	void Update()override;
	void Render(RenderContext& renderContext)override;

	void Input();
	void SelectScale();

	MenuResult GetResult()const { return m_result; }

private:
	SpriteRender m_backTitleRender;
	SpriteRender m_endGameRender;
	SpriteRender m_restartRender;
	SpriteRender m_soundTestRender;

	MenuResult m_result = MenuResult::None;

	int m_selectIndex = 0;
};
