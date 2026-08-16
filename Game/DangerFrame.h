#pragma once

#include "graphics/SpriteRender.h"
#include "Player.h"

class DangerFrame : public IGameObject
{
public:
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;
    void Reset();

private:
    SpriteRender m_sprite;

    Player* m_player = nullptr;

    bool m_isDraw = false;

    // 脈動用
    float m_timer = 0.0f;

    // 表示時間
    float m_displayTimer = 0.0f;

    float m_delayTimer = 0.0f;

    bool m_isStarted = false;

    bool m_prevDangerState = false;

};