#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "SoundOption.h"
#include "BGMManager.h"
#include "Operation.h"

namespace {

	const Vector3 TITLE_POS = { 0.0f,0.0f,0.0f };
	
}

bool Title::Start()
{
	m_spriteRender.Init("Assets/Sprite/Title.DDS", 1920, 1080);
	m_spriteRender.SetPosition({TITLE_POS});
	m_canInput = false;

	m_BGM = FindGO<BGMManager>("bgmManager");
	if (m_BGM == nullptr)
	{
		m_BGM = NewGO<BGMManager>(0, "bgmManager");
		m_BGM->Init();
	}
	

	m_BGM->Play(BGM_GameClear);

	return true;
}


void Title::Update()
{
    m_spriteRender.Update();

    if (!m_canInput)
    {
        m_canInput = true;
        return;
    }

    //…のちに統合版に加える…//
    if (g_pad[0]->IsPress(enButtonSelect))
    {
	    if (Game* game = FindGO<Game>("game"))
	    {
		game->ResetGame();
	    }
	    DeleteGO(this);
    }


    // ★ 操作説明へ（Yボタン）
    if (g_pad[0]->IsPress(enButtonY))
    {
    	Game::SetState(GameState::Title);
        m_operation = NewGO<Operation>(0, "operation");
	    DeleteGO(this);
    }

    // ★ サウンドオプションへ（Xボタン）
    if (g_pad[0]->IsPress(enButtonX))
    {

	   Game::SetState(GameState::Title);   // ← 超重要
	   Game::SetState(GameState::SoundTest);   // ← 正しい状態

	   NewGO<SoundOption>(0, "soundOption");
	   DeleteGO(this);
    }   
}

void Title::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}