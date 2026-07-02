#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include <cmath>   // powf
#include "Item.h"
#include "ItemSpawner.h"


bool GameCamera::Start()
{

    m_normalCameraPos =
    {
        0.0f,
        250.0f,
        -600.0f
    };

    m_toCameraPos.Set(0.0f, 50.0f, -200.0f);
    m_player = FindGO<Player>("player");
    m_spawner = FindGO<ItemSpawner>("itemSpawner");

    ::g_camera3D->SetNear(1.0f);
    ::g_camera3D->SetFar(20000.0f);

    m_currentPhase = GamePhase::Start;
    m_isZooming = false;
    m_zoomT = 0.0f;

    m_isTestZoom = false;
    m_testZoomTimer = 0.0f;

    m_currentCameraPos = m_normalCameraPos;
    return true;
}


void GameCamera::Update()
{
    if (!UpdatePlayer())
        return;
  
    UpdateQTECamera();
   
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
        {
            return false;
        }
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

    if (phase == m_currentPhase)
    {
        return;
    }

    m_currentPhase = phase;

    switch (phase)
    {
    case GamePhase::MovePhase:

        // 移動フェーズ用カメラ
        //MoveCameraForMovePhase();
        break;

    case GamePhase::AfterMove:

        m_zoomFromOffset = m_toCameraPos;

        m_zoomToOffset =
        {
            0.0f,
            150.0f,
            -200.0f
        };

        m_useElastic = false;

        m_isZooming = true;
        m_zoomT = 0.0f;

        break;

    case GamePhase::QTEMove:

        break;

    default:
        break;
    }
}


/// <summary>
/// 移動中はカメラを固定し、傘回し開始時にズームする処理
/// </summary>
/// 
void GameCamera::ApplyCamera()
{
    // 固定カメラにする条件

    bool isFixedCamera =
        (
            Game::GetPhase() == GamePhase::MovePhase ||
            Game::GetPhase() == GamePhase::AfterMove ||
            Game::GetPhase() == GamePhase::SpecialMove||
            Game::GetPhase() == GamePhase::QTEMove
            )
        &&
        !m_player->m_itemOnUmbrella;

 

    if (isFixedCamera)
    {
       
        Vector3 fixedTarget =
        {
            0.0f,
            80.0f,
            300.0f
        };

        ::g_camera3D->SetTarget(fixedTarget);
        ::g_camera3D->SetPosition(m_normalCameraPos);

        m_currentCameraPos = m_normalCameraPos;

        ::g_camera3D->Update();
        return;
    }

    // ===== 傘回し中 =====

    Vector3 playerPos = m_player->GetPosition();

    Vector3 target = playerPos;
    target.y += 80.0f;

    ::g_camera3D->SetTarget(target);

    Vector3 targetPos = m_normalCameraPos;

    if (m_player->m_itemOnUmbrella)
    {
        targetPos =
        {
            playerPos.x + m_toCameraPos.x,
            playerPos.y + m_toCameraPos.y,
            playerPos.z + m_toCameraPos.z
        };
    }

   
    if (Game::GetPhase() == GamePhase::QTEMove)
    {
        m_currentCameraPos = targetPos;
    }
    else
    {
        m_currentCameraPos +=
            (targetPos - m_currentCameraPos) * 0.08f;
    }

 

    ::g_camera3D->SetPosition(m_currentCameraPos);
    ::g_camera3D->Update();
}

/// <summary>
/// カメラのズームを更新する関数。ズーム中でない場合は何もしません。ズーム中の場合、m_zoomTを0.04f増加させ、1.0f以上になった場合は1.0fに固定し、ズーム中フラグをfalseにします。その後、イージング関数を使用して、m_toCameraPosをm_zoomFromOffsetとm_zoomToOffsetの間で補間します。
/// </summary>
void GameCamera::UpdateZoom()
{

    if (!m_isZooming)
    {
        return;
    }



    m_zoomT += 0.04f;

    if (m_zoomT >= 1.0f)
    {
        m_zoomT = 1.0f;
        m_isZooming = false;
    }

    float ease = 0.0f;

    if (m_useElastic)
    {
        ease = EaseOutElastic(m_zoomT);
    }
    else
    {
        ease = EaseInOutCubic(m_zoomT);
    }


    m_toCameraPos =
        m_zoomFromOffset * (1.0f - ease) +
        m_zoomToOffset * ease;

}

/// <summary>
/// QTE用のカメラ演出を更新する関数。ズーム中でない場合、QTE用の寄りカメラのオフセットを設定し、
/// m_toCameraPosがすでにその位置にある場合は何もしません。
/// </summary>
/// void GameCamera::UpdateQTECamera()

void GameCamera::UpdateQTECamera()
{
    if (!m_spawner)return;

    Item* item = m_spawner->GetCurrentItem();
    if (!item)return;

    bool isQTEActive = item->IsQTEActive();

    // QTE用の寄りカメラ
    Vector3 targetOffset = isQTEActive ? Vector3(0.0f, 80.0f, -150.0f) : Vector3(0.0f, 150.0f, -250.0f);

    if (!m_isZooming && (m_toCameraPos - targetOffset).Length() > 1.0f)
    {
        m_zoomFromOffset = m_toCameraPos;
        m_zoomToOffset = targetOffset;


        m_isZooming = true;
        m_zoomT = 0.0f;

       m_useElastic = isQTEActive;

       

    }
}


/// <summary>
/// カメラの初期化を行う関数。m_toCameraPosを通常時のカメラオフセットにリセットし、m_currentPhaseをStartに設定し、ズーム関連のフラグとタイマーをリセットします。
/// </summary>
void GameCamera::Reset()
{
    m_toCameraPos.Set(0.0f, 150.0f, -200.0f);
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