#pragma once
class Circle :public IGameObject
{
public:
	bool Start()override;
	void Update()override;
	void Render(RenderContext& renderContext)override;

	//円の位置設定
	//外部から座標を渡して位置を変更
	void SetPosition(const Vector3& pos);

	//表示の切り替え
	//falseにすると描画されなくなる
	void SetVisible(bool visible) { m_isVisible = visible; }

	//半径を取得
	float GetRadius()const { return m_radius; }

	//現在の位置を取得
	//const参照で返すことでコピーコスト削減
	const Vector3& GetPosition()const { return m_position; }

	//円が表示状態かどうか取得
	bool IsVisible()const { return m_isVisible; }

private:
	ModelRender m_modelRender;					//円のモデル
	Vector3 m_position = { 0.0f,-10.0f,0.0f };	//円の現在位置
	bool m_isVisible = true;					//表示するかどうかのフラグ
	float m_radius = 50.0f;						//円の半径
};

