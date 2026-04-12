#pragma once
#include "QTEButton.h"

class QTE : public IGameObject
{
public:
	QTE() {}
	~QTE() {}
	bool Start();
	void Update();
    

    // QTE開始
    void StartQTE(ButtonType button);

    // 状態取得（必要なら）
    bool IsActive() const { return m_state != State::Idle; }

private:
    enum class State
    {
        Idle,      // 何もしていない
        Playing,   // 入力待ち
        Success,   // 成功演出中
        Failure    // 失敗演出中
    };

    State m_state = State::Idle;

    QTEButton* m_qteButton = nullptr;
};



