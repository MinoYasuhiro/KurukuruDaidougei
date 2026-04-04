#pragma once
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

};

