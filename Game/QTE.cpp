#include "stdafx.h"
#include "QTE.h"

bool QTE::Start()
{
    m_qteButton = NewGO<QTEButton>(0);
    m_state = State::Idle;
    return true;
}

void QTE::StartQTE(ButtonType button)
{
    m_qteButton->StartQTE(button);
    m_state = State::Playing;
}

void QTE::Update()
{
    if (m_state != State::Playing)
        return;

    if (!m_qteButton->IsFinished())
        return;

    if (m_qteButton->IsSuccess())
    {
        m_state = State::Success;
        // 成功演出・SEをここで呼べる
    }
    else
    {
        m_state = State::Failure;
        // 失敗演出・SEをここで呼べる
    }
}
