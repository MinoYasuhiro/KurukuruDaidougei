#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include <cmath>   // powf


bool GameCamera::Start()
{
    m_toCameraPos.Set(0.0f, 50.0f, -200.0f);
    m_player = FindGO<Player>("player");

    g_camera3D->SetNear(1.0f);
    g_camera3D->SetFar(20000.0f);

    m_currentPhase = GamePhase::Start;
    m_isZooming = false;
    m_zoomT = 0.0f;

    m_isTestZoom = false;
    m_testZoomTimer = 0.0f;

    return true;
}

void GameCamera::Update()
{
    if (!UpdatePlayer())
        return;
    if (Game::GetPhase() == GamePhase::QTEMove)
    {
        UpdateQTECamera();
    }
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
    if (!m_player)
    {
        m_player = FindGO<Player>("player");
        if (!m_player)
            return false;
    }
    return true;
}

/// <summary>
/// カメラの位置をゲームのフェーズに応じて更新する関数。現在のフェーズが前回のフェーズと同じ場合や、
/// ズーム中の場合は何もしません。フェーズがMovePhaseに変わった場合は、MoveCameraForMovePhase関数を呼び出して、
/// 移動フェーズ用のカメラ位置にズームします。フェーズがAfterMoveに変わった場合は、m_isTestZoomをtrueにして、
/// m_testZoomTimerを0.0fにリセットし、MoveCameraBehindPlayer関数を呼び出して、プレイヤーの背後にカメラを移動します。
/// </summary>
void GameCamera::UpdatePhase()
{
    GamePhase phase = Game::GetPhase();

    if (phase == m_currentPhase || m_isZooming)
        return;


    m_currentPhase = phase;

    switch (phase)
    {
    case GamePhase::MovePhase:
        MoveCameraForMovePhase();
        break;

    case GamePhase::AfterMove:
        m_isTestZoom = true;
        m_testZoomTimer = 0.0f;
        break;

    default:
        break;
    }
}
/// <summary>
/// カメラのズームの更新。ズーム中は、m_zoomFromOffsetからm_zoomToOffsetにイージングされた値でm_toCameraPosが更新されていきます。
/// </summary>
void GameCamera::UpdateZoom()
{
    if (!m_isZooming)
        return;

    m_zoomT += 0.04f;
    if (m_zoomT >= 1.0f)
    {
        m_zoomT = 1.0f;
        m_isZooming = false;
    }

    float ease;

    if (m_useElastic)
        ease = EaseOutElastic(m_zoomT);
    else
        ease = EaseInOutCubic(m_zoomT);
    m_toCameraPos =
        m_zoomFromOffset * (1.0f - ease) +
        m_zoomToOffset * ease;
    
}
/// <summary>
/// カメラのズームインを開始する関数。m_zoomFromOffsetに現在のカメラオフセット、m_zoomToOffsetにズームイン後のカメラオフセットを設定し、m_isZoomingをtrueにして、m_zoomTを0.0fにリセットします。
/// </summary>
void GameCamera::ZoomIn()
{
    if (m_isZooming)
        return;

    m_zoomFromOffset = m_toCameraPos;
    m_zoomToOffset = Vector3(0.0f, 50.0f, -200.0f);

    m_isZooming = true;
    m_zoomT = 0.0f;
}


void GameCamera::UpdateQTECamera()
{
    if (m_isZooming) return;

    // QTE用の寄りカメラ
    Vector3 targetOffset = Vector3(0.0f, 60.0f, -120.0f);

    // すでにその位置なら何もしない

    if ((m_toCameraPos - targetOffset).Length() < 1.0f)
        return;


    m_zoomFromOffset = m_toCameraPos;
    m_zoomToOffset = targetOffset;

    m_isZooming = true;
    m_zoomT = 0.0f;

    m_useElastic = true;
}


void GameCamera::UpdateTestZoom()
{
    if (!m_isTestZoom || m_isZooming)
        return;

    m_testZoomTimer += 1.0f / 60.0f;

    if (m_testZoomTimer >= 3.0f)
    {
        ZoomIn();
        m_isTestZoom = false;
    }
}
/// <summary>
/// カメラの位置とターゲットを更新して、カメラに反映させる関数。m_playerの位置をターゲットにして、m_toCameraPosのオフセットを加えた位置にカメラが配置されるようになっています。
/// </summary>
void GameCamera::ApplyCamera()
{
    if (!m_player)
        return;

    Vector3 target = m_player->m_position;
    target.y += 80.0f;

    g_camera3D->SetTarget(target);
    g_camera3D->SetPosition(target + m_toCameraPos);
    g_camera3D->Update();
}

/// <summary>
/// 通常時のカメラ位置から、移動フェーズのカメラ位置にズームするための関数。m_zoomFromOffsetに現在のカメラオフセット、m_zoomToOffsetに移動フェーズのカメラオフセットを設定し、m_isZoomingをtrueにして、m_zoomTを0.0fにリセットします。
/// </summary>
void GameCamera::MoveCameraForMovePhase()
{
    m_zoomFromOffset = m_toCameraPos;
    m_zoomToOffset = Vector3(0.0f, 100.0f, -250.0f);

    m_isZooming = true;
    m_zoomT = 0.0f;
}
/// <summary>
/// カメラの位置をプレイヤーの背後に移動するための関数。m_zoomFromOffsetに現在のカメラオフセット、m_zoomToOffsetにプレイヤーの背後に配置されるカメラオフセットを設定し、m_isZoomingをtrueにして、m_zoomTを0.0fにリセットします。プレイヤーの背後に配置されるカメラオフセットは、現在のカメラオフセットを180度回転させた位置になります。
/// </summary>
void GameCamera::MoveCameraBehindPlayer()
{
    m_zoomFromOffset = m_toCameraPos;

    m_useElastic = true;

    Vector3 to = m_toCameraPos;

    Quaternion qRot;
    qRot.SetRotationDeg(Vector3::AxisY, 180.0f);
    qRot.Apply(to);

    to.y = m_toCameraPos.y;
    m_zoomToOffset = to;

    m_isZooming = true;
    m_zoomT = 0.0f;
}
/// <summary>
/// カメラの初期化を行う関数。m_toCameraPosを通常時のカメラオフセットにリセットし、m_currentPhaseをStartに設定し、ズーム関連のフラグとタイマーをリセットします。
/// </summary>
void GameCamera::Reset()
{
    m_toCameraPos.Set(0.0f, 50.0f, -200.0f);
    m_currentPhase = GamePhase::Start;
    m_isZooming = false;
    m_zoomT = 0.0f;

    m_isTestZoom = false;
    m_testZoomTimer = 0.0f;
}
/// <summary>
/// カメラのズームのイージング関数。0.0fから1.0fの範囲でtを渡すと、イージングされた値が返ってきます。
/// </summary>
/// <param name="t"></param>
/// <returns></returns>
float GameCamera::EaseInOutCubic(float t)
{
    return (t < 0.5f)
        ? 4.0f * t * t * t
        : 1.0f - powf(-2.0f * t + 2.0f, 3.0f) * 0.5f;
}

/// <summary>
/// QTE用のカメラ演出のイージングバネ移動
/// </summary>
/// <param name="t"></param>
/// <returns></returns>
float GameCamera::EaseOutElastic(float t)
{
    const float c4 = (2 * 3.14158f) / 3;

    return t == 0 ? 0 :
        t == 1 ? 1 :
        powf(2, -6 * t) * sinf((t * 6 - 0.5f) * c4) + 1;
}
