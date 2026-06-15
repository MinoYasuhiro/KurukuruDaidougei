#pragma once
class Rule : public IGameObject
{
public:
	Rule() {}
	~Rule() {}
	bool Start();
	void Update();
	void Render(RenderContext& renderContext);

private:
	SpriteRender m_RuleRender;
	Vector3 m_position;
	bool m_isEnding = false;



};

