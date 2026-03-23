#include "stdafx.h"
#include "Game.h"


bool Game::Start()
{
	
	return true;
}

void Game::Update()
{
	m_modelRender.Update();
}

void Game::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
