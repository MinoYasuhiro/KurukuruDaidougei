#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "SoundOption.h"

bool Title::Start()
{
	m_spriteRender.Init("Assets/Sprite/Title.DDS", 1920, 1080);
	m_spriteRender.SetPosition({ 0, 0, 0 });
	m_canInput = false;
	return true;
}

void Title::Update()
{

	m_spriteRender.Update();


	if (!m_canInput)
	{
		m_canInput = true;
		return; // ★ 1フレーム入力無視
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


	// ★ サウンドオプションへ（Xボタン）
	if (g_pad[0]->IsPress(enButtonX))
	{
		Game::SetState(GameState::Pause);
		NewGO<SoundOption>(0, "soundOption");
		DeleteGO(this);
	}

}

void Title::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}