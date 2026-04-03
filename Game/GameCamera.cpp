#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"


bool GameCamera::Start()
{
    // 通常は背後カメラから開始
    m_toCameraPos.Set(0.0f, 125.0f, -250.0f);
    m_player = FindGO<Player>("player");

    g_camera3D->SetNear(1.0f);
    g_camera3D->SetFar(10000.0f);

    m_currentPhase = GamePhase::Start;
    m_isZooming = false;
    m_zoomT = 0.0f;

    return true;
}

void GameCamera::Update()
{
    if (!UpdatePlayer()) return;

    UpdatePhase();
    UpdateZoom();
    UpdateRotate();
    ApplyCamera();
}

bool GameCamera::UpdatePlayer()
{
    if (m_player == nullptr)
    {
        m_player = FindGO<Player>("player");
        if (m_player == nullptr)
            return false;
    }
    return true;
}

void GameCamera::UpdatePhase()
{
    GamePhase phase = Game::GetPhase();

    // ★ フェーズが変わった瞬間だけ反応
    if (phase == m_currentPhase || m_isZooming)
        return;

    m_currentPhase = phase;

    switch (phase)
    {
    case GamePhase::MovePhase:
        MoveCameraForMovePhase();
        break;

    case GamePhase::AfterMove:
        MoveCameraBehindPlayer();
        break;
    }
}

void GameCamera::UpdateZoom()
{
    if (!m_isZooming) return;

    m_zoomT += 0.04f;
    if (m_zoomT >= 1.0f)
    {
        m_zoomT = 1.0f;
        m_isZooming = false;
    }

    float ease = EaseInOutCubic(m_zoomT);
    m_toCameraPos =
        m_zoomFromOffset * (1.0f - ease) +
        m_zoomToOffset * ease;
}

void GameCamera::UpdateRotate()
{
    if (m_isZooming) return;

    float x = g_pad[0]->GetRStickXF();
    float y = g_pad[0]->GetRStickYF();

    Quaternion qRot;
    qRot.SetRotationDeg(Vector3::AxisY, 1.3f * x);
    qRot.Apply(m_toCameraPos);

    Vector3 axisX;
    axisX.Cross(Vector3::AxisY, m_toCameraPos);
    axisX.Normalize();

    qRot.SetRotationDeg(axisX, 1.3f * y);
    qRot.Apply(m_toCameraPos);
}

void GameCamera::ApplyCamera()
{
    Vector3 target = m_player->m_position;
    target.y += 80.0f;

    g_camera3D->SetTarget(target);
    g_camera3D->SetPosition(target + m_toCameraPos);
    g_camera3D->Update();
}

// ===== フェーズ別カメラ動作 =====

void GameCamera::MoveCameraForMovePhase()
{
    m_zoomFromOffset = m_toCameraPos;
    m_zoomToOffset = Vector3(0.0f, 200.0f, -450.0f);

    m_isZooming = true;
    m_zoomT = 0.0f;
}


void GameCamera::MoveCameraBehindPlayer()
{

    // 現在のオフセットを保存
    m_zoomFromOffset = m_toCameraPos;

    // 今のカメラオフセット
    Vector3 to = m_toCameraPos;

    // Y軸周りに180度回転（背後を向く）
    Quaternion rot;
    rot.SetRotationDeg(Vector3::AxisY, 180.0f);
    rot.Apply(to);

    // 高さはそのまま維持
    to.y = m_toCameraPos.y;

    // 補間先として設定
    m_zoomToOffset = to;

    m_isZooming = true;
    m_zoomT = 0.0f;

        
}

float GameCamera::EaseInOutCubic(float t)
{
    return (t < 0.5f)
        ? 4.0f * t * t * t
        : 1.0f - powf(-2.0f * t + 2.0f, 3.0f) * 0.5f;
}

