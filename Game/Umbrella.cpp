//=============================================================
// Umbrella.cpp
// 傘の描画・回転・ランダムズレ処理を行うクラス
//
// 主な処理の流れ：
//   Update() → スピン速度の減衰
//            → 傘回し中(state=Spinning)のズレ計算
//            → 右スティックによる傾き補正
//            → 最終的な回転を合成してモデルに反映
//=============================================================
#include "stdafx.h"
#include "Umbrella.h"
#include "Player.h"
#include "HID/GamePadVibration.h"


Umbrella::Umbrella()
{
}


Umbrella::~Umbrella()
{
}


bool Umbrella::Start()
{
    // --- モデルの初期化 ---
    m_modelRender.Init("Assets/modelData/kasa.tkm");

    // --- 初期座標・スケールの設定 ---
    m_respawnPosition = Vector3(0.0f, 0.0f, 100.0f);
    m_modelRender.SetPosition(m_respawnPosition);
    m_scale = Vector3(1.0f, 1.0f, 1.0f);
    m_modelRender.SetScale(m_scale);

    // --- 関連オブジェクトの取得 ---
    m_player = FindGO<Player>("player");

    return true;
}


void Umbrella::SetSpinSpeed(float speed)
{
    m_spinSpeed = speed;
}


void Umbrella::Reset()
{
    // 回転・速度をすべてリセット
    m_angleX = 0.0f;
    m_angleY = 0.0f;
    m_rotationY = 0.0f;
    m_spinSpeed = 0.0f;

    m_rotation = Quaternion::Identity;
    m_finalRotation = Quaternion::Identity;
    m_modelRender.SetRotation(m_rotation);
}


//void Umbrella::Update()
//{
//    const float deltaTime = 1.0f / 60.0f;
//
//    // --- スピン速度の減衰 ---
//    if (m_isAutoSpin)
//    {
//        m_rotationY += 10.0f;      // 一定速度で回す
//    }
//    else
//    {
//        m_spinSpeed *= 0.96f;
//        m_rotationY += m_spinSpeed;
//    }
//
//    // --- 右スティック入力の取得（傘回し中のみ使用）---
//    float x = 0.0f;
//    float y = 0.0f;
//    if (m_player->m_playerState == Player::enPlayerState_Spinning)
//    {
//        x = g_pad[0]->GetRStickXF();
//        y = g_pad[0]->GetRStickYF();
//
//        // デッドゾーン処理
//        if (fabsf(x) < 0.2f) x = 0.0f;
//        if (fabsf(y) < 0.2f) y = 0.0f;
//    }
//
//    // --- 傘ズレ処理（傘回し中のみ）---
//    if (m_player->m_playerState == Player::enPlayerState_Spinning)
//    {
//        // ① 一定間隔でランダムな力の方向を更新
//        m_offsetNoiseTimer -= deltaTime;
//        if (m_offsetNoiseTimer <= 0.0f)
//        {
//            m_offsetNoiseTimer = 1.5f;  // 次の方向変化まで1.5秒
//            m_offsetForceX = ((float)(rand() % 200) - 100.0f) / 20.0f;
//            m_offsetForceZ = ((float)(rand() % 200) - 100.0f) / 20.0f;
//        }
//
//        // ② ランダム力を速度に加算（ゆっくり加速させる）
//        m_offsetVelX += m_offsetForceX * deltaTime * m_shakePower;
//        m_offsetVelZ += m_offsetForceZ * deltaTime * m_shakePower;
//
//        // ③ 速度を減衰（慣性を持たせてゆったり動かす）
//        m_offsetVelX *= 0.97f;
//        m_offsetVelZ *= 0.97f;
//
//        // ④ 速度をオフセット角度に反映
//        m_offsetAngleX += m_offsetVelX;
//        m_offsetAngleZ += m_offsetVelZ;
//
//        // ⑤ 傾きの大きさを計算
//        float totalAngle = sqrtf(
//            m_offsetAngleX * m_offsetAngleX +
//            m_offsetAngleZ * m_offsetAngleZ);
//
//        // ⑥ 失敗判定：一定角度を超えたら傘回し失敗にする
//        const float failAngle = 40.0f;  // この角度を超えると失敗
//        const float maxAngle = 50.0f;  // 傾きの最大制限
//
//        if (totalAngle >= failAngle)
//        {
//            if (m_player && m_player->m_playerState == Player::enPlayerState_Spinning)
//            {
//                auto vib = NewGO<nsK2EngineLow::GamePadVibration>(0);
//                vib->Init(0, 1.0f, 1.0f);
//                // プレイヤーを失敗状態にしてミスカウントを増やす
//                m_player->m_playerState = Player::enPlayerState_Fail;
//                //m_player->m_playerError++;
//
//                // オフセットをリセット
//                m_offsetAngleX = 0.0f;
//                m_offsetAngleZ = 0.0f;
//                m_offsetVelX = 0.0f;
//                m_offsetVelZ = 0.0f;
//            }
//            return;
//        }
//
//        // ⑦ 最大角度を超えないようにクランプ
//        if (totalAngle > maxAngle)
//        {
//            m_offsetAngleX = m_offsetAngleX / totalAngle * maxAngle;
//            m_offsetAngleZ = m_offsetAngleZ / totalAngle * maxAngle;
//        }
//    }
//    else
//    {
//        // 傘回し以外の状態ではズレをすべてリセット
//        m_offsetAngleX = 0.0f;
//        m_offsetAngleZ = 0.0f;
//        m_offsetVelX = 0.0f;
//        m_offsetVelZ = 0.0f;
//        m_offsetNoiseTimer = 0.0f;
//    }
//
//    // --- 右スティックによる傾き補正 ---
//    // スティックを倒した方向に傘を戻す力を加える
//    float stickLen = sqrtf(x * x + y * y);
//    if (stickLen >= 0.2f)
//    {
//        m_offsetVelX += (y * 6.0f) * deltaTime;  // 上下入力 → X軸方向の力
//        m_offsetVelZ += (-x * 6.0f) * deltaTime;  // 左右入力 → Z軸方向の力
//    }
//
//    // --- 回転の合成 ---
//
//    // ズレ角度からX・Z軸の傾きを作成
//    Quaternion rotX;
//    rotX.SetRotationDeg(Vector3::AxisX, m_offsetAngleX);
//    Quaternion rotZ;
//    rotZ.SetRotationDeg(Vector3::AxisZ, m_offsetAngleZ);
//
//    // X・Zの傾きを合成
//    Quaternion tilt = rotX * rotZ;
//
//    // 傾いたY軸を求めてスピン回転に使う
//    Vector3 axis = Vector3::AxisY;
//    tilt.Apply(axis);
//
//    // スピン回転を作成
//    Quaternion spin;
//    spin.SetRotationDeg(axis, m_rotationY);
//
//    // スピンと傾きを合成して最終的な回転を決定
//    Quaternion finalRot = spin * tilt;
//    m_modelRender.SetRotation(finalRot);
//    m_modelRender.Update();
//}

void Umbrella::Update()
{
    const float deltaTime = 1.0f / 60.0f;

    // --- スピン速度の減衰 ---
    if (m_isAutoSpin)
    {
        m_rotationY += 10.0f;      // 一定速度で回す
    }
    else
    {
        m_spinSpeed *= 0.96f;
        m_rotationY += m_spinSpeed;
    }

    // --- LB・RB入力の取得 ---
    float input = 0.0f;

    if (m_player->m_playerState == Player::enPlayerState_Spinning)
    {
        if (g_pad[0]->IsPress(enButtonLB1))
        {
            input = -1.0f;
        }

        if (g_pad[0]->IsPress(enButtonRB1))
        {
            input = 1.0f;
        }
    }

    // --- 傘ズレ処理（傘回し中のみ）---
    if (m_player->m_playerState == Player::enPlayerState_Spinning)
    {
        // ① 一定間隔でランダムな力の方向を更新
        // 左右どちらかへゆっくり倒れる
        m_offsetNoiseTimer -= deltaTime;

        if (m_offsetNoiseTimer <= 0.0f)
        {
            m_offsetNoiseTimer = 1.5f;

            if (rand() % 2 == 0)
            {
                m_offsetForceX = 4.0f;      // 右へ倒れる
            }
            else
            {
                m_offsetForceX = -4.0f;     // 左へ倒れる
            }
        }

        // ② ランダム力を速度に加算（左右のみ）
        m_offsetVelX += m_offsetForceX * deltaTime * m_shakePower;

        // ③ 速度を減衰
        m_offsetVelX *= 0.97f;

        // ④ 速度をオフセット角度に反映
        m_offsetAngleX += m_offsetVelX;

        float totalAngle = fabsf(m_offsetAngleX);

        // ⑥ 失敗判定：一定角度を超えたら傘回し失敗にする
        const float failAngle = 40.0f;  // この角度を超えると失敗
        const float maxAngle = 50.0f;  // 傾きの最大制限

        if (totalAngle >= failAngle)
        {
            if (m_player && m_player->m_playerState == Player::enPlayerState_Spinning)
            {
                auto vib = NewGO<nsK2EngineLow::GamePadVibration>(0);
                vib->Init(0, 1.0f, 1.0f);
                // プレイヤーを失敗状態にしてミスカウントを増やす
                m_player->m_playerState = Player::enPlayerState_Fail;
                //m_player->m_playerError++;

                // オフセットをリセット
                m_offsetAngleX = 0.0f;
                m_offsetVelX = 0.0f;
            }
            return;
        }

        // ⑦ 最大角度を超えないようにクランプ
        if (totalAngle > maxAngle)
        {
            m_offsetAngleX = (m_offsetAngleX / totalAngle) * maxAngle;
        }
    }
    else
    {
        // 傘回し以外の状態ではズレをすべてリセット
        m_offsetAngleX = 0.0f;
        m_offsetVelX = 0.0f;
        m_offsetNoiseTimer = 0.0f;
    }

    // --- LB・RBでバランス補正 ---
    const float balancePower = 5.0f;

    m_offsetVelX -= input * balancePower * deltaTime;

    // --- 回転の合成 ---

    // X方向だけ傾ける
    Quaternion tilt;
    tilt.SetRotationDeg(Vector3::AxisZ, m_offsetAngleX);

    // 傾いたY軸を取得
    Vector3 axis = Vector3::AxisY;
    tilt.Apply(axis);

    // スピン回転
    Quaternion spin;
    spin.SetRotationDeg(axis, m_rotationY);

    // 最終回転
    Quaternion finalRot = spin * tilt;

    m_finalRotation = spin * tilt;

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
    // 傘回し開始時に角度・回転をリセット
    m_angleX = 0.0f;
    m_angleY = 0.0f;
    m_rotation = Quaternion::Identity;
    m_finalRotation = Quaternion::Identity;
}

void Umbrella::SetAutoSpin(bool flag)
{
    m_isAutoSpin = flag;
}