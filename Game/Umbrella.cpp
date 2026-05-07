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

    // ===== 傾き =====
    m_angleX += y * 1.2f;
    m_angleY += -x * 1.2f;

    Quaternion rotX;
    rotX.SetRotationDeg(Vector3::AxisX, m_angleX);

    Quaternion rotZ;
    rotZ.SetRotationDeg(Vector3::AxisZ, m_angleY);

    Quaternion tilt = rotX * rotZ;

    // 傾いたY軸
    Vector3 axis = Vector3::AxisY;
    tilt.Apply(axis);

    // スピン
    Quaternion spin;
    spin.SetRotationDeg(axis, m_rotationY);

    // 親回転なし
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
