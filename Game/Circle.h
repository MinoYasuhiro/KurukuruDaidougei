#pragma once
class Circle:public IGameObject
{
public:
	bool Start()override;
	void Update()override;
	void Render(RenderContext& renderContext)override;

	//円の位置設定
	void SetPosition(const Vector3& pos);
	//表示の切り替え
	void SetVisible(bool visible) { m_isVisible = visible; }
	float GetRadius()const { return m_radius; }

	const Vector3& GetPosition()const { return m_position; }

private:
	ModelRender m_modelRender;					//円のモデル
	Vector3 m_position = { 0.0f,-10.0f,0.0f };	//円の現在位置
	bool m_isVisible = true;					//表示するかどうかのフラグ
	float m_radius = 50.0f;
};

