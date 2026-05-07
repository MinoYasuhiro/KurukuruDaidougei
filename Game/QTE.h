#pragma once
#include "QTEButton.h"

class QTE : public IGameObject
{
public:
    bool Start()
    {
        m_button = NewGO<QTEButton>(0, "qteButton");
        m_state = State::Idle;
        return true;
    }

    void StartQTE(ButtonType button, float limitTime)
    {
        m_button->StartQTE(button, limitTime);
        m_state = State::Playing;
    }

    void Update()
    {
        if (m_state != State::Playing)
            return;

        if (!m_button->IsFinished())
            return;

        m_state = m_button->IsSuccess()
            ? State::Success
            : State::Failure;
    }

    bool IsFinished() const
    {
        return m_state == State::Success || m_state == State::Failure;
    }

    bool IsSuccess() const
    {
        return m_state == State::Success;
    }

private:
    enum class State
    {
        Idle,
        Playing,
        Success,
        Failure
    };

    State m_state = State::Idle;
    QTEButton* m_button = nullptr;
};


