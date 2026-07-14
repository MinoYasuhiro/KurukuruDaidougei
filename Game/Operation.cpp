#include "stdafx.h"
#include "Operation.h"
#include "Title.h"
#include "Game.h"
#include "SEManager.h"

namespace {

	const Vector3 POSITION = Vector3(0.0f, 0.0f, 0.0f);

}

bool Operation::Start()
{
	
	///操作説明
	m_SpriteRender.Init("Assets/Sprite/gamePad.DDS", 1920.0f, 1080.0f);
	m_SpriteRender.SetPosition(POSITION);

	SEManager::Init();

	return true;
}

void Operation::Update()
{
	m_SpriteRender.Update();

	/// タイトルへ(Bボタンで）
	if (g_pad[0]->IsPress(enButtonB))
	{
		Game::SetState(GameState::Title);
		NewGO<Title>(0, "title");

		SEManager::Play(SE_return,false);

		DeleteGO(this);  
		return;          
	}
}

void Operation::Render(RenderContext& rc)
{
	m_SpriteRender.Draw(rc);
}