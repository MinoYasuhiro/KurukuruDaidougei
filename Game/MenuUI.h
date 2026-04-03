#pragma once
class MenuUI:public IGameObject
{
public:
	MenuUI();
	~MenuUI();
	bool Start()override;
	void Update()override;
	void Render(RenderContext&renderContext)override;

	void Input();
	void SelectScale();

private:
	SpriteRender m_backTitleRender;
	SpriteRender m_endGameRender;
	SpriteRender m_restartRender;
	SpriteRender m_soundTestRender;
	FontRender m_testFont;	//テスト用

	int m_selectIndex = 0;
};

