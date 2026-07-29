#include "stdafx.h"
#include "GameOver.h"
#include "Game.h"
#include "Title.h"
#include "Player.h"
#include "GameOverMenu.h"
#include "BGMManager.h"

bool GameOver::Start()
{
	m_SpriteRender.Init("Assets/Sprite/GameOver.DDS", 1920, 1080);

    m_canInput = false;
   
    m_BGM = FindGO<BGMManager>("bgmManager");
    

   if (m_BGM == nullptr)  
   {
            
       m_BGM = NewGO<BGMManager>(0, "bgmManager");
            
       m_BGM->Init();
            
   }
   
   m_BGM->Play(BGM_GameOver);
    
   if (m_menu == nullptr)
   {
            
       m_menu = NewGO<GameOverMenu>(10, "overMenu");
            
   }
    
	//m_player = FindGO<Player>("player");

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