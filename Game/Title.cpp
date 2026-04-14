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

	if (g_pad[0]->IsPress(enButtonA))
	{
		//Gameクラスのオブジェクトを作成。
		NewGO<Game>(0, "game");
		DeleteGO(this);
	}
}

void Title::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}