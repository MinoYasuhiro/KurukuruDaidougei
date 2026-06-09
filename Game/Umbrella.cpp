#include "stdafx.h"
#include "Umbrella.h"
#include "Player.h"

Umbrella::Umbrella()
{
}

Umbrella::~Umbrella()
{
}

bool Umbrella::Start()
{
    m_modelRender.Init("Assets/modelData/kasa.tkm");

    m_respwnPosition = Vector3(0.0f, 0.0f, 100.0f);
    m_modelRender.SetPosition(m_respwnPosition);
    m_scale = Vector3(1.0f, 1.0f, 1.0f);
    m_modelRender.SetScale(m_scale);

	m_player = FindGO<Player>("player");

    return true;
}

void Umbrella::SetSpinSpeed(float speed)
{
    m_spinSpeed = speed;
}

void Umbrella::Reset()
{
    m_angleX = 0.0f;
    m_angleY = 0.0f;
    m_rotationY = 0.0f;
    m_spinSpeed = 0.0f;

    m_rotation = Quaternion::Identity;
    m_modelRender.SetRotation(m_rotation);
}


void Umbrella::Update()
{
    const float deltaTime = 1.0f / 60.0f;

    // スピン
    m_spinSpeed *= 0.96f;
    m_rotationY += m_spinSpeed;

    // 入力
    float x = 0.0f;
    float y = 0.0f;
    if (m_player->m_playerState == 3)
    {
        x = g_pad[0]->GetRStickXF();
        y = g_pad[0]->GetRStickYF();
        if (fabsf(x) < 0.2f) x = 0.0f;
        if (fabsf(y) < 0.2f) y = 0.0f;
    }

    // ===== 傘ズレ処理(state=3のみ) =====
    if (m_player->m_playerState == 3)
    {
        // ① ランダム力を0.5秒ごとに変える
        m_offsetNoiseTimer -= deltaTime;
        if (m_offsetNoiseTimer <= 0.0f)
        {
            m_offsetNoiseTimer = 1.5f;  // 0.5f→1.5f (力の方向が変わる間隔を長く)
            m_offsetForceX = ((float)(rand() % 200) - 100.0f) / 20.0f;
            m_offsetForceZ = ((float)(rand() % 200) - 100.0f) / 20.0f;
        }

        // ② 速度にランダム力を加算
        m_offsetVelX += m_offsetForceX * deltaTime * 0.4f;  // *0.4f追加(加速をゆっくりに)
        m_offsetVelZ += m_offsetForceZ * deltaTime * 0.4f;

        // ③ 減衰
        m_offsetVelX *= 0.97f;  // 0.92f→0.97f (慣性を強くしてゆったり動く)
        m_offsetVelZ *= 0.97f;

        // ④ オフセット角度を更新 ← これが抜けていた
        m_offsetAngleX += m_offsetVelX;
        m_offsetAngleZ += m_offsetVelZ;

        // ⑤ 最大傾き制限 & 失敗判定
        const float failAngle = 40.0f;  // 25.0f→40.0f (判定を甘く)
        const float maxAngle = 50.0f;  // 30.0f→50.0f (大きく傾けるように)

        float totalAngle = sqrtf(
            m_offsetAngleX * m_offsetAngleX +
            m_offsetAngleZ * m_offsetAngleZ);

        if (totalAngle >= failAngle)
        {
            if (m_player && m_player->m_playerState == 3) // state=3のときだけ
            {
                m_player->m_playerState = 1;
                m_player->m_playerError++;

                // オフセットもリセット
                m_offsetAngleX = 0.0f;
                m_offsetAngleZ = 0.0f;
                m_offsetVelX = 0.0f;
                m_offsetVelZ = 0.0f;
            }
            return;
        }

        if (totalAngle > maxAngle)
        {
            m_offsetAngleX = m_offsetAngleX / totalAngle * maxAngle;
            m_offsetAngleZ = m_offsetAngleZ / totalAngle * maxAngle;
        }
    }
    else
    {
        // state=3以外はオフセットをリセット
        m_offsetAngleX = 0.0f;
        m_offsetAngleZ = 0.0f;
        m_offsetVelX = 0.0f;
        m_offsetVelZ = 0.0f;
        m_offsetNoiseTimer = 0.0f;
    }

    // 右スティックでオフセット速度を中心方向に引き戻す
    float stickLen = sqrtf(x * x + y * y);
    if (stickLen >= 0.2f)
    {
        // スティックの方向をそのまま力にする
        m_offsetVelX += (y * 6.0f) * deltaTime;  // 上下入力→X軸
        m_offsetVelZ += (-x * 6.0f) * deltaTime; // 左右入力→Z軸
    }

    // ★ズレのみで傾きを計算
    Quaternion rotX;
    rotX.SetRotationDeg(Vector3::AxisX, m_offsetAngleX);
    Quaternion rotZ;
    rotZ.SetRotationDeg(Vector3::AxisZ, m_offsetAngleZ);

    Quaternion tilt = rotX * rotZ;

    // 傾いたY軸
    Vector3 axis = Vector3::AxisY;
    tilt.Apply(axis);

    // スピン
    Quaternion spin;
    spin.SetRotationDeg(axis, m_rotationY);

    Quaternion finalRot = spin * tilt;
    m_modelRender.SetRotation(finalRot);
    m_modelRender.Update();
}
void Umbrella::Render(RenderContext& rc)
{
    m_modelRender.Draw(rc);
}

void Umbrella::SetPosition(const Vector3& pos)
{
    m_position = pos;
    m_modelRender.SetPosition(pos);
}

void Umbrella::SetRotation(const Quaternion& rot)
{
    m_rotation = rot;
    m_modelRender.SetRotation(rot);
}
void Umbrella::OnStartSpin()
{
    m_angleX = 0.0f;
    m_angleY = 0.0f;

	m_rotation = Quaternion::Identity;
}
