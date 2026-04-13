#pragma once
class Umbrella : public IGameObject
{
public:
	Umbrella();
	~Umbrella();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	// ★追加
	void SetSpinSpeed(float speed);

	//メンバ変数。
	Vector3 m_position;	//傘の座標
	Vector3 m_rot;		//傘の回転
	ModelRender m_modelRender;	//傘の描画

	// ★追加
	float m_spinSpeed = 0.0f;
	float m_rotationY = 0.0f;
	Vector3 m_scale = Vector3(1.0f, 1.0f, 1.0f);
	Vector3 m_respwnPosition; //傘の復活ポジション
};