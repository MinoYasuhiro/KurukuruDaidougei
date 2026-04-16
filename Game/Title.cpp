#include "stdafx.h"
#include "Title.h"
#include "Game.h"


bool Title::Start()
{
	m_spriteRender.Init("Assets/Sprite/Title.DDS", 1920, 1080);
	return true;
}

void Title::Update()
{

	m_spriteRender.Update();


	//…のちに統合版に加える…//
	if (g_pad[0]->IsPress(enButtonSelect))
	{
		if (Game* game = FindGO<Game>("game"))
		{
			DeleteGO(game);
		}
		//Gameクラスのオブジェクトを作成。
		Game::SetState(GameState::Playing);
		NewGO<Game>(0, "game");
		DeleteGO(this);
	}
}

void Title::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}