#pragma once
class BackGround : public IGameObject
{
public:
	BackGround() {}
	~BackGround() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	Vector3 m_position;
	PhysicsStaticObject m_physicsStaticObject;
	ModelRender m_modelRender;
	BackGround* m_backGround;

};

