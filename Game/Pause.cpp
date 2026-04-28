#include "stdafx.h"
#include "Pause.h"
#include "Title.h"
#include "Game.h"
#include "MenuUI.h"
#include "SoundUI.h"
#include "SoundTest.h"

bool Pause::Start()
{
    
    // Menu 表示
    m_Sprite.Init("Assets/Sprite/Menu.DDS", 100, 100);
    m_Sprite.SetPosition(Vector3(0, 200, 0)); // ★ 手前
    m_Sprite.SetScale(Vector3(1.5f, 1.5f, 1.5f));

    // Pause 表示
    m_SpriteRender.Init("Assets/Sprite/Pause.DDS", 100, 100);
    m_SpriteRender.SetPosition(Vector3(0, -200, 0)); // ★ 手前
    m_SpriteRender.SetScale(Vector3(1.5f, 1.5f, 1.5f));

    //Menu背景　表示
    m_PauseRender.Init("Assets/Sprite/PauseBackground.DDS", 1920, 1080);
    m_PauseRender.SetPosition(Vector3(0, 0, 0));
    
    m_menuUI = new MenuUI();    //メニューUIの生成
    m_menuUI->Start();          //メニューUIの初期化

    return true;
}


void Pause::Update()
{

    //m_SpriteRender.Update();
    m_Sprite.Update();
    m_PauseRender.Update();
    m_SpriteRender.Update();

    m_menuUI->Update();

    switch (m_menuUI->GetResult())
    {
        //タイトルに戻る
    case MenuResult::BackTitle:
        Game::SetState(GameState::Title);
        NewGO<Title>(0, "title");
        delete m_menuUI;
        m_menuUI = nullptr;
        DeleteGO(this);
        break;
        //リスタート
    case MenuResult::Restart:
        Game::SetState(GameState::Playing);
        delete m_menuUI;
        m_menuUI = nullptr;
        DeleteGO(this);
        break;
        //ゲームをやめる
    case MenuResult::EndGame:

        delete m_menuUI;
        m_menuUI = nullptr;
        DeleteGO(this);
        break;
        //サウンドオプション
    case MenuResult::SoundOption:

        if (Game* game = FindGO<Game>("game"))
            game->RequestSoundTest();
      

        delete m_menuUI;
        m_menuUI = nullptr;
        DeleteGO(this);
        break;
    }
}

void Pause::Render(RenderContext& rc)
{
    m_PauseRender.Draw(rc);
    m_SpriteRender.Draw(rc);
    m_Sprite.Draw(rc);

    m_menuUI->Render(rc);
}

