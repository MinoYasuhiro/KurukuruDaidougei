#include "stdafx.h"
#include "GameClear.h"

bool GameClear::Start()
{
	m_SpriteRender.Init("Assets/Sprite/GameClear.DDS", 1920 ,1080);
	return true;
}

void GameClear::Update()
{
	m_SpriteRender.Update();
}


void GameClear::Render(RenderContext& rc)
{
	m_SpriteRender.Draw(rc);
}