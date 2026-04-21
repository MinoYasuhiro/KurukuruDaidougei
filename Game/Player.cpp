#include "stdafx.h"
#include "Player.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
#include "Umbrella.h"
#include "SEManager.h"
#include "Game.h"

Player::Player()
{
}

Player::~Player()
{
}

bool Player::Start()
{
    //// アニメーション読み込み
    //m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
    //m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
    //m_animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
    //m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
    //m_animationClips[enAnimationClip_Jump].Load("Assets/animData/jump.tka");
    //m_animationClips[enAnimationClip_Jump].SetLoopFlag(false);
    //m_animationClips[enAnimationClip_Run].Load("Assets/animData/run.tka");
    //m_animationClips[enAnimationClip_Run].SetLoopFlag(true);

    //// モデル
    //m_modelRender.Init("Assets/modelData/unityChan.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisY);

    m_playerAnimationState[enPlayerAnimationState_Idle].Load("Assets/animData/PlayerIdle.tka");
    m_playerAnimationState[enPlayerAnimationState_Idle].SetLoopFlag(true);
    m_playerAnimationState[enPlayerAnimationState_Run].Load("Assets/animData/PlayerRun.tka");
    m_playerAnimationState[enPlayerAnimationState_Run].SetLoopFlag(true);
    m_playerAnimationState[enPlayerAnimationState_Rotation].Load("Assets/animData/PlayerRotation.tka");
    m_playerAnimationState[enPlayerAnimationState_Rotation].SetLoopFlag(true);
    m_playerAnimationState[enPlayerAnimationState_success].Load("Assets/animData/PlayerSuccess.tka");
    m_playerAnimationState[enPlayerAnimationState_success].SetLoopFlag(true);

    m_NewModelRender.Init("Assets/modelData/Player2.tkm", m_playerAnimationState, enPlayerAnimationState_Num, enModelUpAxisZ);
    //m_NewModelRender.SetPosition({ 0, 0, 0 });
    // 傘生成
    m_umbrella = NewGO<Umbrella>(0);

    // キャラコン
    m_characterController.Init(25.0f, 75.0f, m_position);

    // 初期モード
    number = 1;
    m_state = 0;
    m_prevNumber = 1;

    m_runSound = NewGO<SoundSource>(0);
    m_runSound->Init(SE_run);

    return true;
}


void Player::Reset()
{
    m_position = m_startPos;
}

void Player::Update()
{
    m_prevNumber = number;

    /*if (number > 3 and g_pad[0]->IsTrigger(enButtonSelect))
    {
        number = 1;
    }*/

    // =========================
    // モード切り替え（Aボタン）
    // =========================
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        number += 1;
        if (number > 3)
        {
            number = 1;
        }
    }

    if (m_prevNumber != 1 && number == 1)
    {
        if (Game* game = FindGO<Game>("game"))
        {
            game->RequestMovePhase();
        }
    }

    // =========================
    // モードごとの処理
    // =========================
    if (number == 1)
    {
        m_playerState = 0;

        // 移動モード
        Move();
        Rotation();
        ManageState();
        //PlayAnimation();
        PlayAnimation2();

        // 傘は止める
        m_umbrella->SetSpinSpeed(0.0f);
    }
    if (number == 2)
    {
        // 傘回しモード

        // プレイヤーを止める
        m_moveSpeed = Vector3::Zero;

        // アニメーションは待機固定
        m_playerState = 3;
        //PlayAnimation();
        PlayAnimation2();

        // 傘回転
        float spin = CalcStickRotationSpeed();
        m_umbrella->SetSpinSpeed(spin);
    }

    bool isMoving =
        fabsf(m_moveSpeed.x) >= 0.001f ||
        fabsf(m_moveSpeed.z) >= 0.001f;

    if (isMoving && !m_wasMoving)
    {
        m_runSound->Play(true);
        m_isRunSEPlaying = true;
    }
    if (!isMoving && m_isRunSEPlaying)
    {
        m_runSound->Stop();
        m_isRunSEPlaying = false;
    }

    if (number == 3)
    {
        m_moveSpeed = Vector3::Zero;

        m_playerState = 4;
        PlayAnimation2();

    }

    // モデル更新
    //m_modelRender.Update();
    m_NewModelRender.Update();

    int boneNo = m_NewModelRender.FindBoneID(L"Middle_r");

    if (boneNo != -1)
    {
        Bone* bone = m_NewModelRender.GetBone(boneNo);

        Vector3 pos;
        Quaternion rot;
        Vector3 scale;

        bone->CalcWorldTRS(pos, rot, scale);

        m_umbrella->SetPosition(pos);
        m_umbrella->SetRotation(rot);
    }
}

void Player::Move()
{
    m_moveSpeed.x = 0.0f;
    m_moveSpeed.z = 0.0f;

    Vector3 stickL;
    stickL.x = g_pad[0]->GetLStickXF();
    stickL.y = g_pad[0]->GetLStickYF();

    Vector3 forward = g_camera3D->GetForward();
    Vector3 right = g_camera3D->GetRight();

    forward.y = 0.0f;
    right.y = 0.0f;

    right *= stickL.x * 800.0f;
    forward *= stickL.y * 800.0f;

    m_moveSpeed += right + forward;

    if (m_characterController.IsOnGround())
    {
        m_moveSpeed.y = 0.0f;

        /* if (g_pad[0]->IsPress(enButtonSelect) || g_pad[0]->IsPress(enButtonLB2) || g_pad[0]->IsPress(enButtonRB2))
         {
             m_moveSpeed.y = 240.0f;
         }*/
    }
    else
    {
        m_moveSpeed.y -= 6.0f;
    }

    m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);
    m_modelRender.SetPosition(m_position);
    m_NewModelRender.SetPosition(m_position);
}

void Player::Rotation()
{
    if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
    {
        m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
        m_modelRender.SetRotation(m_rotation);
        m_NewModelRender.SetRotation(m_rotation);
    }
}

void Player::ManageState()
{
    // すでにステート3なら何もしない
    if (m_playerState == 3)
    {
        return;
    }

    if (m_characterController.IsOnGround() == false)
    {
        m_playerState = 1;
        return;
    }



    if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
    {
        m_playerState = 2;
    }
    else
    {
        m_playerState = 0;
    }


}

//void Player::PlayAnimation()
//{
//    switch (m_playerState) {
//    case 0:
//        m_modelRender.PlayAnimation(enAnimationClip_Idle);
//        break;
//    case 1:
//        m_modelRender.PlayAnimation(enAnimationClip_Jump);
//        break;
//    case 2:
//        m_modelRender.PlayAnimation(enAnimationClip_Run);
//        break;
//    }
//}

void Player::PlayAnimation2()
{
    switch (m_playerState) {
    case 0:
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_Idle);
        break;
    case 1:
        //m_NewModelRender.PlayAnimation(enPlayerAnimationState_Jump);
        break;
    case 2:
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_Run);
        break;
    case 3:
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_Rotation);
        break;
    case 4:
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_success);
        break;
    }


}


float Length(Vector2 v)
{
    return sqrtf(v.x * v.x + v.y * v.y);
}

Vector2 Normalize(Vector2 v)
{
    float len = sqrtf(v.x * v.x + v.y * v.y);

    if (len > 0.0001f)
    {
        v.x /= len;
        v.y /= len;
    }
    return v;
}

float Dot(Vector2 a, Vector2 b)
{
    return a.x * b.x + a.y * b.y;
}

float Clamp(float v, float min, float max)
{
    if (v < min) return min;
    if (v > max) return max;
    return v;
}

float Player::CalcStickRotationSpeed()
{
    Vector2 current;
    current.x = g_pad[0]->GetLStickXF();
    current.y = g_pad[0]->GetLStickYF();

    float len = sqrtf(current.x * current.x + current.y * current.y);

    if (len < 0.2f)
    {
        m_prevStick = current;
        return 0.0f;
    }

    // 正規化
    current.x /= len;
    current.y /= len;

    float prevLen = sqrtf(m_prevStick.x * m_prevStick.x + m_prevStick.y * m_prevStick.y);

    if (prevLen > 0.0001f)
    {
        m_prevStick.x /= prevLen;
        m_prevStick.y /= prevLen;
    }
    // 内積
    float dot = current.x * m_prevStick.x + current.y * m_prevStick.y;

    dot = Clamp(dot, -1.0f, 1.0f);

    float angle = acosf(dot);

    // 外積（2D） 
    float cross = m_prevStick.x * current.y - m_prevStick.y * current.x;

    if (cross < 0)
    {
        angle = -angle;
    }

    m_prevStick = current;

    return angle * 20.0f;
}


void Player::Render(RenderContext& rc)
{
    //ユニティーちゃんを表示する。
   // m_modelRender.Draw(rc);

    m_NewModelRender.Draw(rc);
}