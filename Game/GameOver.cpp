#include "stdafx.h"
#include "GameOver.h"
#include "Game.h"
#include "Title.h"


bool GameOver::Start()
{
	m_SpriteRender.Init("Assets/Sprite/GameOver.DDS", 1920, 1080);
	return true;
}

void GameOver::Update()
{

    m_SpriteRender.Update();

    // ★ 入力持ち越し対策
    if (!m_canInput)
    {
        m_canInput = true;
        return;
    }
    
    //タイトル
    if (g_pad[0]->IsPress(enButtonB))
    {
        if (Game* game = FindGO<Game>("game"))
        {
            game->RequestTitle();
        }
        
        DeleteGO(this);
    }

}

void GameOver::Render(RenderContext& rc)
{
	m_SpriteRender.Draw(rc);
}