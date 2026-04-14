#pragma once
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


};

