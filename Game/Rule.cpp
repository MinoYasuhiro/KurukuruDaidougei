#include "stdafx.h"
#include "Rule.h"
#include "Title.h"
#include "Game.h"

namespace RULE
{
	const Vector3 POSITION = { 0.0f,0.0f,0.0f };
}

bool Rule::Start()
{
	m_RuleRender.Init("Assets/Sprite/Rule1.DDS", 1920, 1080);
	m_RuleRender.SetPosition(Vector3(RULE::POSITION));

	m_isEnding = false;
	return true;
}

void Rule::Update()
{
	m_RuleRender.Update();

	// ★① 先に遷移処理（次フレーム）
	if (m_isEnding)
	{
		Game::SetState(GameState::Title);
		NewGO<Title>(0, "title");
	
		m_isDead = true; // ここで削除
		return;
	}

	// ★② 入力でフラグだけ立てる
	if (g_pad[0]->IsPress(enButtonB))
	{
		m_isEnding = true;
		return;
	}


}

void Rule::Render(RenderContext& rc)
{
	m_RuleRender.Draw(rc);
}