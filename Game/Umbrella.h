#pragma once
class Player;
class Umbrella : public IGameObject
{
public:
	Umbrella();
	~Umbrella();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	void Reset();
	void OnStartSpin();
	Quaternion GetRotation() const;

	// ★追加
	void SetSpinSpeed(float speed);
	void SetPosition(const Vector3& pos);
	void SetRotation(const Quaternion& rot);

	//メンバ変数。
	Vector3 m_position;	//傘の座標
	Vector3 m_rot;		//傘の回転
	ModelRender m_modelRender;	//傘の描画
	Quaternion m_rotation;

	// ★追加
	float m_spinSpeed = 0.0f;
	float m_rotationY = 0.0f;
	Vector3 m_scale = Vector3(1.0f, 1.0f, 1.0f);
	Vector3 m_respwnPosition; //傘の復活ポジション
	Player* m_player = nullptr; //プレイヤーへのポインタ
	Vector3 m_forward = Vector3::AxisZ; // 傘の向き
	float m_angleY = 0.0f;
	float m_angleX = 0.0f;

	//追加
	// 傘ズレ関連
	float m_offsetAngleX = 0.0f;      // ランダムズレ角度X
	float m_offsetAngleZ = 0.0f;      // ランダムズレ角度Z
	float m_offsetVelX = 0.0f;        // ズレの速度X
	float m_offsetVelZ = 0.0f;        // ズレの速度Z
	float m_offsetForceX = 0.0f;      // ランダム力X
	float m_offsetForceZ = 0.0f;      // ランダム力Z
	float m_offsetNoiseTimer = 0.0f;  // ランダム力切り替えタイマー
};

