#pragma once
class BGMManager;
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
	Vector3 m_position;
	bool m_canInput = false;

	BGMManager* m_BGM = nullptr;
};

