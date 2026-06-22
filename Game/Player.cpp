//=============================================================
// Player.cpp
// プレイヤーの移動・傘回し・ステート管理を行うクラス
//
// 主な処理の流れ：
//   Update() → ManageState()でステート決定
//            → PlayerAction()で行動実行
//            → SoundPlay()でSE再生
//=============================================================
#include "stdafx.h"
#include "Player.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
#include "Umbrella.h"
#include "SEManager.h"
#include "Game.h"
#include "Item.h"
#include "CoinBox.h"
#include "Arrow.h"
#include "Circle.h"

// 傘回しゲームのパラメータ定数（Player.hのk_系と対応）
const float Player::k_spinTimeLimit = 3.0f;   // 傘回しの制限時間
const float Player::k_gameStartDelay = 3.0f;   // ゲーム開始までの待機時間
const float Player::k_gameOverRunTime = 3.0f;  // ゲームオーバー後の走り時間


Player::Player()
{
}

Player::~Player()
{
}

bool Player::Start()
{

    // --- アニメーションの読み込み ---
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


    // --- モデルの初期化 ---
    m_NewModelRender.Init("Assets/modelData/Player2.tkm", m_playerAnimationState, enPlayerAnimationState_Num, enModelUpAxisZ);


    // --- 傘の生成 ---
    m_umbrella = NewGO<Umbrella>(0);


    // --- 変数の初期化 ---
    number = 1;
    m_prevNumber = 1;
    m_prevPlayerState = -1;
    m_playerState = 0;
    m_finishRot.SetRotationY(Math::DegToRad(-90.0f));


    // --- UIの初期化 ---
    m_font.SetText(L"");
    m_font.SetPosition(0.0f, 0.0f, 0.0f);
    m_font.SetColor({ 1.0f,1.0f,1.0f,1.0f });
    m_font.SetScale(1.0f);


    m_arrow = NewGO<Arrow>(0);

    m_itemOnUmbrella = false;
    m_isRunSEPlaying = false;

    // --- キャラクターコントローラーの初期化 ---
    m_characterController.Init(10.0f, 50.0f, m_position);

    m_startPos = m_position;

    return true;
}


void Player::Reset()
{
    m_playerState = enPlayerState_Idle;

    m_playerClear = 0;   
    m_playerError = 0;   

    m_spinTimer = 0.0f;
    m_spinCount = 0;
    m_itemOnUmbrella = false;

    m_position = m_startPos;
    m_rotation = Quaternion::Identity;

    m_characterController.SetPosition(m_position);

    m_NewModelRender.SetPosition(m_position);
    m_NewModelRender.SetRotation(m_rotation);
    m_NewModelRender.Update();

    m_umbrella->Reset();

    m_gameStartTimer = 0.0f;
    m_canPlayerMove = false;

    m_spinSpeed = 0.0f;

    m_prevStick = Vector2::Zero;
    m_prevStick2 = Vector2::Zero;

    m_inputCooldown = 0.0f;

    m_isRunSEPlaying = false;

    m_gameOverRunTimer = 0.0f;
}

void Player::Update()
{
    if (Game::GetState() != GameState::Playing)return;
    // --- 必要なオブジェクトの取得 ---
    if (!m_game)
    {
        m_game = FindGO <Game>("game");
        if (!m_game)return;
    }


    // --- 必要なオブジェクトの取得 ---
    if (!m_umbrella)
    {
        m_umbrella = FindGO<Umbrella>("umbrella");
    }


    // --- ゲーム中でなければ処理しない ---
    if (m_game->GetState() != GameState::Playing)
    {
        m_isRunSEPlaying = false;
        return;
    }


    // --- ゲーム開始タイマー（一定時間後に操作可能になる）---
    m_gameStartTimer += 1.0f / 60.0f;

    if (m_gameStartTimer >= k_gameStartDelay)
    {
        m_canPlayerMove = true;
    }


    // --- デバッグ表示（スピン回数）---
    wchar_t text[256];
    swprintf_s(text, 256, L"Spin Count : %d", m_spinCount);

    m_font.SetText(text);

    m_prevNumber = number;


    if (m_prevNumber != 1 && number == 1)
    {
        if (Game* game = FindGO<Game>("game"))
        {
            game->RequestMovePhase();
        }
    }


    // --- メインの更新処理 ---
    if (m_canPlayerMove)
    {
        ManageState();     // 状態決定
        PlayerAction();    // 行動
        SoundPlay();       // サウンド再生
    }
    else
    {
        // 開始前は待機状態
        m_playerState = enPlayerState_Idle;
    }


    // --- ステートが変わったときにアニメーションを切り替える ---
    if (m_prevPlayerState != m_playerState)
    {
        if (m_playerState == enPlayerState_Spinning)
        {
            if (m_umbrella)
            {
                m_umbrella->OnStartSpin();
            }
        }

        PlayAnimation2();
    }


    // --- モデル・傘の位置更新 ---
    m_NewModelRender.Update();

    m_prevPlayerState = m_playerState;


    // ★傘の位置を更新
    int boneNo = m_NewModelRender.FindBoneID(L"Middle_r");

    if (boneNo != -1)
    {
        Bone* bone = m_NewModelRender.GetBone(boneNo);

        Vector3 pos;
        Quaternion rot;
        Vector3 scale;

        bone->CalcWorldTRS(pos, rot, scale);

        // 手首位置だけ追従
        if (m_umbrella)
        {
            m_umbrella->SetPosition(pos);
        }
    }


    //円が未取得、または削除されている場合は再取得
    if (!m_circle || m_circle->IsDead())
    {
        m_circle = FindGO<Circle>("circle");
    }


    // --- 矢印が存在する場合のみ処理 ---
    if (m_arrow)
    {
        //円が存在し、削除されておらず、表示状態の場合
        if (m_circle && !m_circle->IsDead() && m_circle->IsVisible())
        {
            //現在位置と円の中心との距離を計算
            float dist = (m_circle->GetPosition() - m_position).Length();

            //プレイヤーが円の中にいる場合
            if (dist <= m_circle->GetRadius())
            {
                //矢印を非表示
                m_arrow->SetActive(false);
            }
            else
            {
                //円の外にいる場合は矢印を表示
                m_arrow->SetActive(true);
                //円の方向を指すように矢印の向きを設定
                m_arrow->SetDirection(m_position, m_circle->GetPosition());
            }
        }
        else
        {
            //円が存在しない・非表示などの場合は矢印も非表示
            m_arrow->SetActive(false);
        }
    }
}


//サウンドの再生。
void Player::SoundPlay()
{
    if (Game::GetState() != GameState::Playing)
    {
        SEManager::StopLoop(SE_run);
        m_isRunSEPlaying = false;
        return;
    }


    if (m_playerState != enPlayerState_Run)
    {
        if (m_isRunSEPlaying)
        {
            SEManager::StopLoop(SE_run);
            m_isRunSEPlaying = false;
        }
        return;
    }


    bool isMoveInput =
        fabsf(g_pad[0]->GetLStickXF()) >= 0.1f ||
        fabsf(g_pad[0]->GetLStickYF()) >= 0.1f;


    if (isMoveInput && !m_isRunSEPlaying)
    {
        SEManager::Play(SE_run, true);
        m_isRunSEPlaying = true;
    }


    else if (!isMoveInput && m_isRunSEPlaying)
    {
        SEManager::StopLoop(SE_run);
        m_isRunSEPlaying = false;
    }
}


//移動処理。
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

    right *= stickL.x * 400.0f;
    forward *= stickL.y * 400.0f;

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
    m_NewModelRender.SetPosition(m_position);
}


//回転処理。
void Player::Rotation()
{
    if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
    {
        m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
        m_NewModelRender.SetRotation(m_rotation);
    }
}


//状態管理。
void Player::ManageState()
{
    Vector2 stick;
    stick.x = g_pad[0]->GetLStickXF();
    stick.y = g_pad[0]->GetLStickYF();

    switch (m_playerState)
    {
    case enPlayerState_Idle: // 待機
    {
        // ★クリア判定
        if (m_playerClear >= k_clearCountToWin)
        {
            m_playerState = enPlayerState_GameClear1;
            return;
        }

        // ★ゲームオーバー判定
        if (m_playerError >= k_errorCountToGameOver)
        {
            m_playerState = enPlayerState_GameOver;
            return;
        }

        // 傘回し開始
        if (m_itemOnUmbrella)
        {
            m_playerState = enPlayerState_Spinning;
        }
        // 移動開始
        else if (fabsf(stick.x) >= 0.1f || fabsf(stick.y) >= 0.1f)
        {
            m_playerState = enPlayerState_Run;
        }

        break;
    }


    case enPlayerState_Run: // 移動
    {
        // 傘回し開始
        if (m_itemOnUmbrella)
        {
            m_playerState = enPlayerState_Spinning;
        }
        // 入力がないなら待機
        else if (fabsf(stick.x) < 0.1f &&
            fabsf(stick.y) < 0.1f)
        {
            m_playerState = enPlayerState_Idle;
        }

        break;
    }


    case enPlayerState_Fail: // 傘回し失敗中
    case enPlayerState_Success: // 傘回し成功中

        // アニメ終了待ち
        if (!m_NewModelRender.IsPlayingAnimation())
        {
            EndUmbrellaSpin();

            m_playerState = enPlayerState_Idle;

        }

        break;


    case enPlayerState_GameClear1: // クリア中
        if (!m_NewModelRender.IsPlayingAnimation())
        {
            m_playerState = enPlayerState_GameClear2;
        }
        break;


    case enPlayerState_GameOver: // ゲームオーバー中
        if (!m_NewModelRender.IsPlayingAnimation())
        {
            m_playerState = enPlayerState_GameOverRun;
        }
        break;


    case enPlayerState_Stop:
        // 何もしない（停止）
        break;
    }
}


//プレイヤーのアクション。
void Player::PlayerAction()
{
    switch (m_playerState)
    {
    case enPlayerState_Idle:
        //通常時待機のアクション
        break;

    case enPlayerState_Fail:
        //通常時失敗のアクション
        break;

    case enPlayerState_Run:
        //走るアクション
        Move();
        Rotation();
        break;

    case enPlayerState_Spinning:
    {
        // 正面向き処理
        Quaternion targetRot;
        targetRot.SetRotationY(0.0f);

        Quaternion rot;
        rot.Slerp(0.08f, m_rotation, targetRot);

        m_rotation = rot;
        m_NewModelRender.SetRotation(m_rotation);

        // 傘回し
        float inputSpin = CalcStickRotationSpeed();

        m_spinSpeed += inputSpin * 0.11f;
        m_spinSpeed *= 0.96f;

        if (m_spinSpeed > 20.0f) m_spinSpeed = 30.0f;
        if (m_spinSpeed < -20.0f) m_spinSpeed = -30.0f;

        if (m_umbrella)
        {
            m_umbrella->SetSpinSpeed(m_spinSpeed);
        }

        SpinCount();

        // ★タイマー加算
        m_spinTimer += 1.0f / 60.0f;

        // ★成功判定（25回）
        if (m_spinCount >= k_spinCountToSuccess)
        {
            m_playerState = enPlayerState_Success;
            m_playerClear++;

            if (CoinBox* coin = FindGO<CoinBox>("coinBox"))
            {
                coin->AddCoin();
            }
        }

        // ★失敗判定（3秒）
        else if (m_spinTimer >= k_spinTimeLimit)
        {
            m_playerState = enPlayerState_Fail;
            m_playerError++;
        }
    }
    break;

    case enPlayerState_Success:
        //通常時成功のアクション
        break;

    case enPlayerState_QTEFail:
        //QTE失敗のアクション
        break;

    case enPlayerState_QTESuccess:
        //QTE成功のアクション
        break;

    case enPlayerState_GameClear1:
        //ゲームクリアのアクション1
        break;

    case enPlayerState_GameClear2:
        //ゲームクリアのアクション2
        break;

    case enPlayerState_GameOver:
        //ゲームオーバーのアクション
        break;

    case enPlayerState_GameOverRun:
    {
        // ゲームオーバー後の走り
        m_moveSpeed = Vector3(-400.0f, 0.0f, 0.0f);

        m_position += m_moveSpeed * (1.0f / 60.0f);

        m_NewModelRender.SetRotation(m_finishRot);
        m_NewModelRender.SetPosition(m_position);

        // タイマー加算
        m_gameOverRunTimer += 1.0f / 60.0f;

        // 3秒後に待機へ
        if (m_gameOverRunTimer >= k_gameOverRunTime)
        {
            m_playerError = 0;     // ←追加
            m_itemOnUmbrella = false; // ←追加
            m_playerState = enPlayerState_Stop;
            m_gameOverRunTimer = 0.0f;
            m_moveSpeed = Vector3::Zero; // ←追加
        }
    }
    break;


    case enPlayerState_Stop:
    {
        // 完全停止
        m_moveSpeed = Vector3::Zero;

        m_NewModelRender.SetPosition(m_position);
    }
    break;
    }
}


//傘回しの回転数保持
void Player::SpinCount()
{
    // 現フレームのスティック入力を取得
    Vector2 current;
    current.x = g_pad[0]->GetLStickXF();
    current.y = g_pad[0]->GetLStickYF();

    float len = sqrtf(current.x * current.x + current.y * current.y);

    // デッドゾーン：入力が小さすぎる場合はゼロ扱い
    if (len < 0.2f)
    {
        current = Vector2(0, 0);
    }

    // 前フレームとの差分（スティックの移動量）を計算
    Vector2 delta;
    delta.x = current.x - m_prevStick2.x;
    delta.y = current.y - m_prevStick2.y;
    float change = sqrtf(delta.x * delta.x + delta.y * delta.y);

    // クールタイムを減らす（連続カウント防止）
    m_inputCooldown -= 1.0f / 60.0f;

    bool isInput = (current.x != 0.0f || current.y != 0.0f);

    // スティックが動いていて、クールタイムが切れていて、入力がある場合のみカウント
    if (change > 0.2f && m_inputCooldown <= 0.0f && isInput)
    {
        m_spinCount++;
        m_inputCooldown = 0.05f;  // 次のカウントまでのインターバル
    }

    m_prevStick2 = current;
}


//アニメーションの再生。
void Player::PlayAnimation2()
{
    switch (m_playerState) {
    case enPlayerState_Idle:
        //通常時待機のアニメーション
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_Idle);
        break;

    case enPlayerState_Fail:
        //通常時失敗のアニメーション
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_Normal);
        break;

    case enPlayerState_Run:
        //走るアニメーション
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_Run);
        break;

    case enPlayerState_Spinning:
        //傘を回すアニメーション
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_Rotation);
        break;

    case enPlayerState_Success:
        //通常時成功のアニメーション
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_Success);
        break;

    case enPlayerState_QTEFail:
        //QTE失敗のアニメーション
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_QTEsippai);
        break;

    case enPlayerState_QTESuccess:
        //QTE成功のアニメーション
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_QTEseikou);
        break;

    case enPlayerState_GameClear1:
        //ゲームクリアのアニメーション1
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_GameClear1);
        break;

    case enPlayerState_GameClear2:
        //ゲームクリアのアニメーション2
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_GameClear2);
        break;

    case enPlayerState_GameOver:
        //ゲームオーバーのアニメーション
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_GameOver);
        break;

    case enPlayerState_GameOverRun:
        //ゲームオーバーの走るアニメーション
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_GameOverRun);
        break;

    case enPlayerState_Stop:
        // 完全停止のアニメーション
        m_NewModelRender.PlayAnimation(enPlayerAnimationState_Idle);
        break;
    }
}


// ベクトルの長さを計算
static float Length(Vector2 v)
{
    return sqrtf(v.x * v.x + v.y * v.y);
}


// ベクトルを正規化
static Vector2 Normalize(Vector2 v)
{
    float len = sqrtf(v.x * v.x + v.y * v.y);

    if (len > 0.0001f)
    {
        v.x /= len;
        v.y /= len;
    }
    return v;
}


// ベクトルの内積を計算
static float Dot(Vector2 a, Vector2 b)
{
    return a.x * b.x + a.y * b.y;
}


static float Clamp(float v, float min, float max)
{
    if (v < min) return min;
    if (v > max) return max;
    return v;
}


// スティックの回転速度を計算
// 前フレームからスティックがどれだけ「回転したか」を角度で返す
float Player::CalcStickRotationSpeed()
{
    // 現フレームのスティック入力を取得
    Vector2 current;
    current.x = g_pad[0]->GetLStickXF();
    current.y = g_pad[0]->GetLStickYF();

    float len = sqrtf(current.x * current.x + current.y * current.y);

    // デッドゾーン：スティックがほぼ中央なら回転なしとして終了
    if (len < 0.2f)
    {
        m_prevStick = current;
        return 0.0f;
    }

    // 現フレームのスティックを正規化（向きだけ取り出す）
    current.x /= len;
    current.y /= len;

    // 前フレームのスティックも正規化
    float prevLen = sqrtf(m_prevStick.x * m_prevStick.x + m_prevStick.y * m_prevStick.y);
    if (prevLen > 0.0001f)
    {
        m_prevStick.x /= prevLen;
        m_prevStick.y /= prevLen;
    }

    // 内積で前フレームと現フレームの「なす角」を求める
    // dot = cos(θ) なので、acosを取ると2ベクトル間の角度になる
    float dot = current.x * m_prevStick.x + current.y * m_prevStick.y;
    dot = Clamp(dot, -1.0f, 1.0f);  // 浮動小数点誤差でacosが壊れないようにクランプ
    float angle = acosf(dot);

    // 外積（2D）で回転方向（時計回り／反時計回り）を判定
    // 外積が負なら時計回り → angleを負にする
    float cross = m_prevStick.x * current.y - m_prevStick.y * current.x;
    if (cross < 0)
    {
        angle = -angle;
    }

    // 今フレームの向きを次フレームの「前フレーム」として保存
    m_prevStick = current;

    // 角度に倍率をかけて回転速度として返す
    return angle * 20.0f;
}


void Player::EndUmbrellaSpin()
{
    m_itemOnUmbrella = false;

    m_spinSpeed = 0.0f;

    m_prevStick = Vector2::Zero;
    m_prevStick2 = Vector2::Zero;

    m_spinCount = 0;
    m_spinTimer = 0.0f;

    m_umbrella->Reset();
}


//描画処理。
void Player::Render(RenderContext& rc)
{
    m_NewModelRender.Draw(rc);
    //m_font.Draw(rc);
}