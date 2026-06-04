#pragma once
class Game;
class MenuUI;
class Pause : public IGameObject
{
public:
	Pause() {}
	~Pause() {}

	bool Start();
	void Update();
	void CleanupMenu();
	void Render(RenderContext& renderContext);

private:
	SpriteRender m_SpriteRender;
	SpriteRender m_Sprite;
	SpriteRender m_PauseRender;
	SpriteRender m_pauseOperation;
	Vector3 m_position;
	Pause* m_pause;
	MenuUI* m_menuUI = nullptr;
};


