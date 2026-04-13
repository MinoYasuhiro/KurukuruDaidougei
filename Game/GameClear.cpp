#include "stdafx.h"
#include "GameClear.h"
#include "Title.h"

bool GameClear::Start()
{
	m_SpriteRender.Init("Assets/Sprite/GameClear.DDS", 1920 ,1080);
	return true;
}

void GameClear::Update()
{
	m_SpriteRender.Update();

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		NewGO<Title>(0, "title");
		DeleteGO(this);
	}
}


void GameClear::Render(RenderContext& rc)
{
	m_SpriteRender.Draw(rc);
}