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
    //// 減衰（回し続けないと止まる）
    //m_spinSpeed *= 0.98f;

    //// 回転
    //m_rotationY += m_spinSpeed;

    //Quaternion rot;
    //rot.SetRotationY(m_rotationY);

    //m_modelRender.SetRotation(rot);

    //if (m_player->m_playerState == 3)
    //{
    //    float x = g_pad[0]->GetRStickXF();
    //    float y = g_pad[0]->GetRStickYF();

    //    // Y軸回転（左右）
    //    Quaternion qRot;
    //    qRot.SetRotationDeg(Vector3::AxisY, 1.5f * x);
    //    qRot.Apply(m_forward);   // ← 傘の向きベクトル

    //    // X軸回転（上下）
    //    Vector3 axisX;
    //    axisX.Cross(Vector3::AxisY, m_forward);
    //    axisX.Normalize();

    //    qRot.SetRotationDeg(axisX, 1.5f * y);
    //    qRot.Apply(m_forward);

    //    m_angleY += x * 2.0f;
    //    m_angleX += y * 2.0f;

    //    Quaternion rotY, rotX;
    //    rotY.SetRotationY(m_angleY);
    //    rotX.SetRotationX(m_angleX);

    //    m_modelRender.SetRotation(rotX * rotY);





    //    m_spinSpeed *= 0.98f;
    //    m_rotationY += m_spinSpeed;

    //    // 入力（傾き）
    //    if (m_player->m_playerState == 3)
    //    {
    //        float x = g_pad[0]->GetRStickXF();
    //        float y = g_pad[0]->GetRStickYF();

    //        m_angleY +=  -x * 0.05f;
    //        m_angleX += y * 0.05f;
    //    }

    //    // 回転作成
    //    Quaternion rotSpin, rotY, rotX;

    //    rotSpin.SetRotationY(m_rotationY); // スピン
    //    rotY.SetRotationY(m_angleY);       // 向き左右
    //    rotX.SetRotationX(m_angleX);       // 向き上下

    //    // 合成（順番大事）
    //    Quaternion finalRot = rotY * rotX * rotSpin;

    //    m_modelRender.SetRotation(finalRot);

    //m_modelRender.Update();




    m_spinSpeed *= 0.96f;
    m_rotationY += m_spinSpeed;

    // 入力（傾き）
    if (m_player->m_playerState == 3)
    {
        float x = g_pad[0]->GetRStickXF();
        float y = g_pad[0]->GetRStickYF();

        // デッドゾーン
        if (fabsf(x) < 0.2f) x = 0.0f;
        if (fabsf(y) < 0.2f) y = 0.0f;

        m_angleY += -x * 0.05f;
        m_angleX += y * 0.05f;
    }

    // ===== ここから回転 =====

    // 傾き
    Quaternion rotY, rotX;
    rotY.SetRotationY(m_angleY);
    rotX.SetRotationX(m_angleX);

    Quaternion tilt = rotY * rotX;

    // 軸を傾ける
    Vector3 axis = Vector3::AxisY;
    tilt.Apply(axis);

    // スピン
    Quaternion spin;
    spin.SetRotationDeg(axis, m_rotationY);

    // 合成
    Quaternion finalRot = spin * tilt;

    m_modelRender.SetRotation(finalRot);

    // =======================

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
}
