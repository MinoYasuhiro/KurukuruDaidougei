
#include "stdafx.h"
#include "GameClear.h"
#include "Title.h"
#include "Game.h"

bool GameClear::Start()
{
    m_SpriteRender.Init("Assets/Sprite/GameClear.DDS", 1920, 1080);
    return true;
}


void GameClear::Update()
{
    m_SpriteRender.Update();

    // 3秒経過したらタイトルへ
    if (g_pad[0]->IsPress(enButtonB))
    {
        if (Game* game = FindGO<Game>("game"))
        {
            game->RequestTitle();
        }
        DeleteGO(this);
    }

}


void GameClear::Render(RenderContext& rc)
{
    m_SpriteRender.Draw(rc);
}
