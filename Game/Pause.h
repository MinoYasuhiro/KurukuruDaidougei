#pragma once
class Game;
class Pause : public IGameObject
{
public:
	Pause() {}
	~Pause() {}

	bool Start();
	void Update();
	void Render(RenderContext& renderContext);

private:
	SpriteRender m_SpriteRender;
	SpriteRender m_Sprite;
	SpriteRender m_PauseRender;
	Vector3 m_position;
	Pause* m_pause;

};

