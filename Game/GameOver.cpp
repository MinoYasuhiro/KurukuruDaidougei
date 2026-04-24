#include "stdafx.h"
#include "GameOver.h"
#include "Game.h"
#include "Title.h"


bool GameOver::Start()
{
	m_SpriteRender.Init("Assets/Sprite/GameOver.DDS", 1920, 1080);

    m_RetryRender.Init("Assets/Sprite/Retry.DDS", 500, 500);
    m_RetryRender.SetPosition(Vector3(0, -200, 0)); // ★ 手前
    m_RetryRender.SetScale(Vector3(1.5f, 1.5f, 1.5f));

	return true;
}

void GameOver::Update()
{

    m_SpriteRender.Update();
    m_RetryRender.Update();
    // ★ 入力持ち越し対策
    if (!m_canInput)
    {
        m_canInput = true;
        return;
    }
    
    //リトライ
    if (g_pad[0]->IsPress(enButtonA))
    {
        if (Game* game = FindGO<Game>("game"))
        {
            game->ResetGame();
        }

        DeleteGO(this);
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
    m_RetryRender.Draw(rc);
}