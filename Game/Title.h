#pragma once
class BGMManager;
class Operation;
class TitleMenu;
class SEManager;
class Title : public IGameObject
{
public:
	Title() {}
	~Title() {}
	bool Start();
	void Update();
	void Render(RenderContext& renderContext);

	
private:
	SpriteRender m_spriteRender;
	SpriteRender m_soundButtonRender;

	Vector3 m_position;
	bool m_canInput = false;

	BGMManager* m_BGM = nullptr;

	Operation* m_operation = nullptr;

	TitleMenu* m_titleMenu = nullptr;

	bool m_inputLock = false;

};

