#pragma once
class Title;
class Game;
class GameOverMenu;
class BGMManager;
class Player;
class GameOver : public IGameObject
{
public:
	GameOver() {}
	~GameOver() {}
	bool Start();
	void Update();
	void Render(RenderContext& renderContext);

private:
	SpriteRender m_SpriteRender;
	Vector3 m_position;
	SpriteRender m_RetryRender;
	bool m_canInput = false;
	Player* m_player = nullptr;
	GameOverMenu* m_menu = nullptr;
	BGMManager* m_BGM = nullptr;



};

