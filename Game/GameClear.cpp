#include "stdafx.h"
#include "GameClear.h"
#include "Title.h"
#include "Game.h"
#include "BGMManager.h"
#include "GameClearMenu.h"


bool GameClear::Start()
{
    m_SpriteRender.Init("Assets/Sprite/GameClear.DDS", 1920, 1080);

    m_canInput = false;

    m_BGM = FindGO<BGMManager>("bgmManager");
    if (m_BGM == nullptr)
    {
        m_BGM = NewGO<BGMManager>(0, "bgmManager");
        m_BGM->Init();
    }

    m_BGM->Play(BGM_GameClear);


    if (m_menu == nullptr)
    {
        m_menu = NewGO<GameClearMenu>(10, "clearMenu");
    }

    return true;
}



void GameClear::Update()
{
    m_SpriteRender.Update();

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
        case ClearMenuResult::Retry:
            Game::SetPrevState(GameState::GameClear);
            FindGO<Game>("game")->ResetGame();

            if (Game* game = FindGO<Game>("game"))
            {
               game->ResetGame(); 
            }
            DeleteGO(m_menu);
            m_isDead = true;
            m_menu = nullptr;
            break;

        case ClearMenuResult::BTitle:
            Game::SetPrevState(GameState::GameClear);

            if (Game* game = FindGO<Game>("game"))
            {
                game->RequestTitle(); 
            }

            DeleteGO(m_menu);
            m_isDead = true;
            m_menu = nullptr; 
            break;
        }
    }


   

}


void GameClear::Render(RenderContext& rc)
{
    m_SpriteRender.Draw(rc);

}
