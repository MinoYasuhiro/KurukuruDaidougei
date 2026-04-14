#include "stdafx.h"
#include "Pause.h"
#include "Title.h"
#include "Game.h"


bool Pause::Start()
{
    // Pause 表示
    m_SpriteRender.Init("Assets/Sprite/Pause.DDS", 100, 100);
    m_SpriteRender.SetPosition(Vector3(0, -200, 0)); // ★ 手前
    m_SpriteRender.SetScale(Vector3(1.5f, 1.5f, 1.5f));

    // Menu 表示
    m_Sprite.Init("Assets/Sprite/Menu.DDS", 100, 100);
    m_Sprite.SetPosition(Vector3(0, 200, 0)); // ★ 手前
    m_Sprite.SetScale(Vector3(1.5f, 1.5f, 1.5f));

    return true;
}


void Pause::Update()
{

    m_SpriteRender.Update();
    m_Sprite.Update();


    // 再開
    if (g_pad[0]->IsPress(enButtonB))
    {
        Game::SetState(GameState::Playing);
        DeleteGO(this);
    }

    if (g_pad[0]->IsPress(enButtonX))
    {
        Game::SetState(GameState::Title);
        NewGO<Title>(0, "title");
        DeleteGO(this);
        
    }

    

}

void Pause::Render(RenderContext& rc)
{
   
	m_SpriteRender.Draw(rc);
    m_Sprite.Draw(rc);
}

