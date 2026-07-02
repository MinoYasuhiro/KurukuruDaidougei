#pragma once
class Player;
class Umbrella : public IGameObject
{
public:
    // --- コンストラクタ・デストラクタ ---
    Umbrella();
    ~Umbrella();

    // --- オーバーライド ---
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

    // --- 状態操作 ---
    void Reset();           // 傘の状態をリセット
    void OnStartSpin();     // 傘回し開始時の初期化

    // --- Getter ---
    Quaternion GetRotation() const
    {
        return m_rotation;
    }

    // --- Setter ---
    void SetSpinSpeed(float speed);             // 傘の回転速度をセット
    void SetPosition(const Vector3& pos);       // 傘の座標をセット
    void SetRotation(const Quaternion& rot);    // 傘の回転をセット
    void SetAutoSpin(bool flag);

    // --- モデル描画 ---
    ModelRender m_modelRender;

    // --- トランスフォーム ---
    Vector3    m_position;                          // 傘の座標
    Quaternion m_rotation;                          // 傘の回転
    Vector3    m_scale = Vector3(1.0f, 1.0f, 1.0f); // 傘のスケール

    // --- 関連オブジェクト ---
    Player* m_player = nullptr;  // プレイヤーへのポインタ

private:
    // --- 傘回転関連 ---
    float m_spinSpeed = 0.0f;  // 傘の回転速度
    float m_rotationY = 0.0f;  // Y軸の累積回転角度
    float m_angleX = 0.0f;  // X軸の回転角度
    float m_angleY = 0.0f;  // Y軸の回転角度
    bool m_isAutoSpin = false;

    // --- 傘ズレ関連（傘回し中にランダムに傾く処理）---
    float m_offsetAngleX = 0.0f;  // ランダムズレ角度X
    float m_offsetAngleZ = 0.0f;  // ランダムズレ角度Z
    float m_offsetVelX = 0.0f;  // ズレの速度X
    float m_offsetVelZ = 0.0f;  // ズレの速度Z
    float m_offsetForceX = 0.0f;  // ランダム力X
    float m_offsetForceZ = 0.0f;  // ランダム力Z
    float m_offsetNoiseTimer = 0.0f;  // ランダム力の切り替えタイマー

    // --- その他 ---
    Vector3 m_respawnPosition;              // 初期配置座標
    Vector3 m_forward = Vector3::AxisZ;     // 傘の向き（現在未使用）
};

