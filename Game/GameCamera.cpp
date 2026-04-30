#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"



bool GameCamera::Start()
{
    // 通常は背後カメラから開始
    m_toCameraPos.Set(0.0f, 50.0f, -200.0f);
    m_player = FindGO<Player>("player");

    g_camera3D->SetNear(1.0f);
    g_camera3D->SetFar(20000.0f);

    m_currentPhase = GamePhase::Start;
    m_isZooming = false;
    m_zoomT = 0.0f;

   

    return true;
}

void GameCamera::Update()
{
    if (!UpdatePlayer()) return;

    // ★ AfterMove 中だけテストズームを動かす
    if (Game::GetPhase() == GamePhase::AfterMove)
    {
        UpdateTestZoom();
    }

    UpdatePhase();
    UpdateZoom();
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
        ///移動フェーズ：少し引いたカメラへズーム
        MoveCameraForMovePhase();
        break;

    case GamePhase::AfterMove:

        // ★ テスト用：ここで開始
        m_isTestZoom = true;
        m_testZoomTimer = 0.0f;

        ///移動後：プレイヤーの背後へ回り込む
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
    m_toCameraPos = m_zoomFromOffset * (1.0f - ease) + m_zoomToOffset * ease;
}

void GameCamera::ZoomIn()
{
    if (m_isZooming) return;  

    // 今の位置（遠い）から
    m_zoomFromOffset = m_toCameraPos;

    // 寄せたい位置（近い）
    m_zoomToOffset = Vector3(0.0f, 50.0f, -200.0f);

    m_isZooming = true;
    m_zoomT = 0.0f;
}

/// <summary>
/// カメラを引き寄せるテスト処理（機能するかの確認のため）
/// </summary>
void GameCamera::UpdateTestZoom()
{
    if (!m_isTestZoom)
        return;

    // ズーム中は待つ
    if (m_isZooming)
        return;

    // 秒数カウント（60fps前提）
    m_testZoomTimer += 1.0f / 60.0f;

    // 3秒経過したら寄せる
    if (m_testZoomTimer >= 3.0f)
    {
        ZoomIn();            // ★ ここで寄せる
        m_isTestZoom = false; // テスト終了
    }
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
    m_zoomToOffset = Vector3(0.0f, 100.0f, -250.0f);

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
    Quaternion qRot;
    qRot.SetRotationDeg(Vector3::AxisY, 180.0f);
    qRot.Apply(to);
    g_camera3D->RotateOriginTarget(qRot);

    // 高さはそのまま維持
    to.y = m_toCameraPos.y;

    // 補間先として設定
    m_zoomToOffset = to;

    m_isZooming = true;
    m_zoomT = 0.0f;

    return;
}

void GameCamera::Reset()
{
    m_toCameraPos.Set(0.0f, 50.0f, -200.0f);
    m_currentPhase = GamePhase::Start;
    m_isZooming = false;
    m_zoomT = 0.0f;


    // ★ テスト用も初期化
    m_isTestZoom = false;
    m_testZoomTimer = 0.0f;

}


float GameCamera::EaseInOutCubic(float t)
{
    return (t < 0.5f)
        ? 4.0f * t * t * t
        : 1.0f - powf(-2.0f * t + 2.0f, 3.0f) * 0.5f;

}

