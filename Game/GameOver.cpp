#include "stdafx.h"
#include "GameOver.h"
#include "Game.h"
#include "Title.h"
#include "Player.h"
#include "GameOverMenu.h"


bool GameOver::Start()
{
	m_SpriteRender.Init("Assets/Sprite/GameOver.DDS", 1920, 1080);

	m_player = FindGO<Player>("player");

    m_menu = NewGO<GameOverMenu>(10, "overMenu");

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

    if (m_menu)
    {
        switch (m_menu->GetResult())
        {
        case OverMenuResult::Retry:
            if (Game* game = FindGO<Game>("game"))
            {
                game->ResetGame();
            }
            DeleteGO(m_menu);
            m_menu = nullptr;
            m_isDead = true;
            return;

        case OverMenuResult::Btitle:
            if (Game* game = FindGO<Game>("game"))
            {
                game->RequestTitle();
            }
            DeleteGO(m_menu);
            m_menu = nullptr;
            m_isDead = true;
            return;
        }

    }

}

void GameOver::Render(RenderContext& rc)
{
	m_SpriteRender.Draw(rc);
}