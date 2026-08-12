#include "stdafx.h"
#include "QTERadialLines.h"

bool QTERadialLines::Start()
{
    m_sprite.Init( "Assets/effect/RadialLines.DDS",  1920.0f,  1080.0f);
    m_sprite.SetPosition(Vector3(0.0f,  0.0f,  0.0f));

    return true;
}

void QTERadialLines::Update()
{
    m_sprite.Update();
}

void QTERadialLines::Render(RenderContext& rc)
{
    if (!m_isActive)
    {
        return;
    }

    m_sprite.Draw(rc);
}

void QTERadialLines::SetActive(bool active)
{
    m_isActive = active;
}