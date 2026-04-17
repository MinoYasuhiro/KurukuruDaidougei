#include "stdafx.h"
#include "Umbrella.h"

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

    return true;
}

void Umbrella::SetSpinSpeed(float speed)
{
    m_spinSpeed = speed;
}

void Umbrella::Update()
{
    // 減衰（回し続けないと止まる）
    m_spinSpeed *= 0.98f;

    // 回転
    m_rotationY += m_spinSpeed;

    Quaternion rot;
    rot.SetRotationY(m_rotationY);

    m_modelRender.SetRotation(rot);

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
