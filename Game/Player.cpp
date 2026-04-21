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
    
    m_playerAnimationState[enPlayerAnimationState_Idle].Load("Assets/animData/PlayerIdle.tka");
    m_playerAnimationState[enPlayerAnimationState_Idle].SetLoopFlag(true);
    m_playerAnimationState[enPlayerAnimationState_Run].Load("Assets/animData/PlayerRun.tka");
    m_playerAnimationState[enPlayerAnimationState_Run].SetLoopFlag(true);
    m_playerAnimationState[enPlayerAnimationState_Rotation].Load("Assets/animData/PlayerRotation.tka");
    m_playerAnimationState[enPlayerAnimationState_Rotation].SetLoopFlag(true);
    m_playerAnimationState[enPlayerAnimationState_Success].Load("Assets/animData/PlayerSuccess.tka");
    m_playerAnimationState[enPlayerAnimationState_Success].SetLoopFlag(false);
    m_playerAnimationState[enPlayerAnimationState_Normal].Load("Assets/animData/normal.tka");
    m_playerAnimationState[enPlayerAnimationState_Normal].SetLoopFlag(false);
    m_playerAnimationState[enPlayerAnimationState_QTEsippai].Load("Assets/animData/QTEsippai.tka");
    m_playerAnimationState[enPlayerAnimationState_QTEsippai].SetLoopFlag(false);
    m_playerAnimationState[enPlayerAnimationState_QTEseikou].Load("Assets/animData/QTEseikou.tka");
    m_playerAnimationState[enPlayerAnimationState_QTEseikou].SetLoopFlag(true);
    m_playerAnimationState[enPlayerAnimationState_GameClear1].Load("Assets/animData/gameClear.tka");
    m_playerAnimationState[enPlayerAnimationState_GameClear1].SetLoopFlag(false);
    m_playerAnimationState[enPlayerAnimationState_GameClear2].Load("Assets/animData/gameClear2.tka");
    m_playerAnimationState[enPlayerAnimationState_GameClear2].SetLoopFlag(false);
    m_playerAnimationState[enPlayerAnimationState_GameOver].Load("Assets/animData/gameover.tka");
    m_playerAnimationState[enPlayerAnimationState_GameOver].SetLoopFlag(false);
    m_playerAnimationState[enPlayerAnimationState_GameOverRun].Load("Assets/animData/PlayerRun.tka");
    m_playerAnimationState[enPlayerAnimationState_GameOverRun].SetLoopFlag(true);

    m_NewModelRender.Init("Assets/modelData/Player2.tkm", m_playerAnimationState, enPlayerAnimationState_Num, enModelUpAxisZ);
    //m_NewModelRender.SetPosition({ 0, 0, 0 });
    // 傘生成
    m_umbrella = NewGO<Umbrella>(0);

    // キャラコン
    m_characterController.Init(10.0f, 50.0f, m_position);

    // 初期モード
    number = 1;
    m_state = 0;
    m_prevNumber = 1;
    m_prevPlayerState = -1;
    m_finishRot.SetRotationY(Math::DegToRad(-90.0f));

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

    ManageState();     // 状態決定
    PlayerAction();    // 行動

    if (m_prevPlayerState != m_playerState)
    {
        PlayAnimation2();
    }

    m_NewModelRender.Update();

    m_prevPlayerState = m_playerState;

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
    Vector2 stick;
    stick.x = g_pad[0]->GetLStickXF();
    stick.y = g_pad[0]->GetLStickYF();

    // 通常時待機と走るの状態遷移
    if (m_playerState == 0 || m_playerState == 2)
    {
        if (fabsf(stick.x) >= 0.1f || fabsf(stick.y) >= 0.1f)
        {
            m_playerState = 2;
        }
        else
        {
            m_playerState = 0;
        }
    }

    // ゲームクリアの状態遷移
    if (m_playerState == 7 && !m_NewModelRender.IsPlayingAnimation())
    {
        m_playerState = 8;
    }

    // ゲームオーバーの状態遷移
    if (m_playerState == 9 && !m_NewModelRender.IsPlayingAnimation())
    {
        m_playerState = 10;
    }
}

void Player::PlayerAction()
{
    switch (m_playerState)
    {
    case 0:
        //通常時待機のアクション
        break;
    case 1:
        //通常時失敗のアクション
        break;
    case 2:
        //走るアクション
        Move();
        Rotation();
        break;
    case 3:
    {
        //傘を回すアクション
        // 傘回転
        float spin = CalcStickRotationSpeed();
        m_umbrella->SetSpinSpeed(spin);
    }
    break;
    case 4:
        //通常時成功のアクション
        break;
    case 5:
        //QTE失敗のアクション
        break;
    case 6:
        //QTE成功のアクション
        break;
    case 7:
        //ゲームクリアのアクション1
        break;
    case 8:
        //ゲームクリアのアクション2
        break;
    case 9:
        //ゲームオーバーのアクション
        break;
    case 10:
        //ゲームオーバーの走るアクション
        m_moveSpeed = Vector3(-400.0f, 0.0f, 0.0f); // 左方向
        m_position += m_moveSpeed * (1.0f / 60.0f);
        m_NewModelRender.SetRotation(m_finishRot);
        m_NewModelRender.SetPosition(m_position);

        break;
    }
}

void Player::PlayAnimation2()
{
    switch (m_playerState) {
    case 0:
        //通常時待機のアニメーション
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_Idle);
        break;
    case 1:
        //通常時失敗のアニメーション
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_Normal);
        break;
    case 2:
        //走るアニメーション
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_Run);
        break;
    case 3:
        //傘を回すアニメーション
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_Rotation);
        break;
    case 4:
        //通常時成功のアニメーション
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_Success);
        break;
    case 5:
        //QTE失敗のアニメーション
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_QTEsippai);
        break;
    case 6:
        //QTE成功のアニメーション
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_QTEseikou);
        break;
    case 7:
        //ゲームクリアのアニメーション1
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_GameClear1);
        break;
    case 8:
        //ゲームクリアのアニメーション2
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_GameClear2);
        break;
    case 9:
        //ゲームオーバーのアニメーション
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_GameOver);
        break;
    case 10:
        //ゲームオーバーの走るアニメーション
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_GameOverRun);
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
    m_NewModelRender.Draw(rc);
}