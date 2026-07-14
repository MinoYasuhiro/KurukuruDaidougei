#pragma once
class SEManager;
class Operation : public IGameObject
{
public:
	Operation() {}
	~Operation() {}
	bool Start();
	void Update();
	void Render(RenderContext& renderContext);

private:
	Vector3 m_position;
	SpriteRender m_SpriteRender;


	
};

