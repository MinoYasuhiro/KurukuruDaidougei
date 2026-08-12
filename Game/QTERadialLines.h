#pragma once

#include "graphics/SpriteRender.h"

class QTERadialLines : public IGameObject
{
public:
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc);

    void SetActive(bool active);

private:
    SpriteRender m_sprite;

    bool m_isActive = false;
};