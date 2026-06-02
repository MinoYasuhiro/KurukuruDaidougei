#include "stdafx.h"
#include "Audience.h"
#include"Player.h"

Audience::Audience()
{
}

Audience::~Audience()
{
}

bool Audience::Start()
{


    m_audienceAnimationState[enAudienceAnimationState_Idle].Load("Assets/animData/AudienceIdle.tka");
    m_audienceAnimationState[enAudienceAnimationState_Idle].SetLoopFlag(true);
    m_audienceAnimationState[enAudienceAnimationState_Clap].Load("Assets/animData/AudienceClap.tka");
    m_audienceAnimationState[enAudienceAnimationState_Clap].SetLoopFlag(true);
    m_audienceAnimationState[enAudienceAnimationState_Clap2].Load("Assets/animData/AudienceClap2.tka");
    m_audienceAnimationState[enAudienceAnimationState_Clap2].SetLoopFlag(true);
    m_audienceAnimationState[enAudienceAnimationState_Clap3].Load("Assets/animData/AudienceClap3.tka");
    m_audienceAnimationState[enAudienceAnimationState_Clap3].SetLoopFlag(true);
    m_audienceAnimationState[enAudienceAnimationState_Clap4].Load("Assets/animData/AudienceClap4.tka");
    m_audienceAnimationState[enAudienceAnimationState_Clap4].SetLoopFlag(true);
    m_audienceAnimationState[enAudienceAnimationState_Clap5].Load("Assets/animData/AudienceClap5.tka");
    m_audienceAnimationState[enAudienceAnimationState_Clap5].SetLoopFlag(true);
    m_audienceAnimationState[enAudienceAnimationState_Clap6].Load("Assets/animData/AudienceClap6.tka");
    m_audienceAnimationState[enAudienceAnimationState_Clap6].SetLoopFlag(true);
    m_audienceAnimationState[enAudienceAnimationState_Happy].Load("Assets/animData/AudienceHappy.tka");
    m_audienceAnimationState[enAudienceAnimationState_Happy].SetLoopFlag(true);
    m_audienceAnimationState[enAudienceAnimationState_Sippai1].Load("Assets/animData/AudienceSippai1.tka");
    m_audienceAnimationState[enAudienceAnimationState_Sippai1].SetLoopFlag(true);
    m_audienceAnimationState[enAudienceAnimationState_Sippai2].Load("Assets/animData/AudienceSippai2.tka");
    m_audienceAnimationState[enAudienceAnimationState_Sippai2].SetLoopFlag(true);

    m_modelRender.Init("Assets/modelData/Audience.tkm", m_audienceAnimationState, enAudienceAnimationState_Num, enModelUpAxisZ);

    m_player = FindGO<Player>("player");


    return true;
}

void Audience::Update()
{
    if (m_player == nullptr)
    {
        m_player = FindGO<Player>("player");
        return;
    }

    m_modelRender.SetPosition(
        m_position);

    m_modelRender.SetRotation(
        m_rotation);

    // プレイヤーステート変更時だけ
    if (m_prevPlayerState != m_player->m_playerState)
    {
        DecideAnimation();
        m_prevPlayerState = m_player->m_playerState;
    }



    m_modelRender.Update();
}

void Audience::Render(RenderContext& rc)
{
    m_modelRender.Draw(rc);
}

void Audience::DecideAnimation()
{
    switch (m_player->m_playerState)
    {
    case 1: // プレイヤー失敗
    case 5: // プレイヤーQTE失敗
    case 9: // プレイヤーゲームオーバー
    case 10: // プレイヤーゲームオーバー走る
    {
        int randAnim = rand() % 2;

        switch (randAnim)
        {
        case 0:
            m_modelRender.PlayAnimation(
                enAudienceAnimationState_Sippai1);
            break;

        case 1:
            m_modelRender.PlayAnimation(
                enAudienceAnimationState_Sippai2);
            break;
        }
        break;
    }

    case 4: // プレイヤー成功
    case 6: // プレイヤーQTE成功
    case 7: // プレイヤーゲームクリア
    case 8: // プレイヤーゲームクリア2
    {
        int randAnim = rand() % 7;

        switch (randAnim)
        {
        case 0:
            m_modelRender.PlayAnimation(
                enAudienceAnimationState_Clap);
            break;

        case 1:
            m_modelRender.PlayAnimation(
                enAudienceAnimationState_Clap2);
            break;

        case 2:
            m_modelRender.PlayAnimation(
                enAudienceAnimationState_Clap3);
            break;

        case 3:
            m_modelRender.PlayAnimation(
                enAudienceAnimationState_Happy);
            break;

        case 4:
            m_modelRender.PlayAnimation(
                enAudienceAnimationState_Clap4);
            break;

        case 5:
            m_modelRender.PlayAnimation(
                enAudienceAnimationState_Clap5);
            break;

        case 6:
            m_modelRender.PlayAnimation(
                enAudienceAnimationState_Clap6);
            break;
        }
        break;
    }

    default: // それ以外全部Idle
        m_modelRender.PlayAnimation(
            enAudienceAnimationState_Idle);
        break;
    }
}

void Audience::SetRotation(
    const Quaternion& rot)
{
    m_rotation = rot;
    m_modelRender.SetRotation(rot);
}

void Audience::SetPosition(const Vector3& pos)
{
    m_position = pos;
    m_modelRender.SetPosition(pos);
}


