#pragma once
class GameClear : public IGameObject
{
public:
	GameClear() {}
	~GameClear() {}
	bool Start();
	void Update();
	void Render(RenderContext& renderContext);

private:
	SpriteRender m_SpriteRender;
	Vector3 m_position;
	


};

