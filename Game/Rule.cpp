#include "stdafx.h"
#include "Rule.h"
#include "Title.h"
#include "Game.h"
#include <algorithm>
#include "SEManager.h"

namespace RULE
{
	const Vector3 POSITION = { 0.0f,0.0f,0.0f };
}

namespace BUTTON
{
	const Vector3 LEFT = { -800.0f,0.0f,0.0f };
	const Vector3 RIGHT = { 800.0f,0.0f,0.0f };
}

Rule::~Rule()
{

  for (auto r : m_rules)
  {
      delete r;
  }
  m_rules.clear();

}

bool Rule::Start()
{
    m_rules.resize(m_maxPage);
    m_rules[0] = new SpriteRender();
    m_rules[0]->Init("Assets/Sprite/Rule1.DDS", 1920, 1080);

    //m_rules[1] = new SpriteRender();
    //m_rules[1]->Init("Assets/Sprite/Rule4.DDS", 1920, 1080);

    m_rules[1] = new SpriteRender();
    m_rules[1]->Init("Assets/Sprite/Rule2.DDS", 1920, 1080);

    m_rules[2] = new SpriteRender();
    m_rules[2]->Init("Assets/Sprite/Rule3.DDS", 1920, 1080);

	for (auto& r : m_rules)
	{
		r->SetPosition(RULE::POSITION);
	}

	m_LeftButtonRender.Init("Assets/Sprite/SelectLeftButton.DDS", 300, 300);
	m_LeftButtonRender.SetPosition(Vector3(BUTTON::LEFT));


	m_RightButtonRender.Init("Assets/Sprite/SelectLightButton.DDS", 300, 300);
	m_RightButtonRender.SetPosition(Vector3(BUTTON::RIGHT));
   

	m_isEnding = false;

	SEManager::Init();
	return true;
}


void Rule::Update()
{

    // ページ更新
    m_rules[m_pageIndex]->Update();
    m_LeftButtonRender.Update();
    m_RightButtonRender.Update();

    UpdateAnimation();
    UpdateInput();
    UpdateExit();
    
}

/// <summary>
/// 矢印ボタンのアニメーション処理
/// </summary>
void Rule::UpdateAnimation()
{
    m_time += g_gameTime->GetFrameDeltaTime();
    float scale = 1.0f + sinf(m_time * 2.5f) * 0.05f;

    if (m_pageIndex > 0)
    {
        m_LeftButtonRender.SetScale({ scale, scale, 1.0f });
    }

    if (m_pageIndex < m_maxPage - 1)
    {
        m_RightButtonRender.SetScale({ scale, scale, 1.0f });
    }

}

/// <summary>
/// 選択処理
/// </summary>
void Rule::UpdateInput()
{

    m_inputTimer -= g_gameTime->GetFrameDeltaTime();

    if (m_inputTimer <= 0.0f)
    {
        if (g_pad[0]->IsPress(enButtonRight))
        {
            m_pageIndex++;

            if (m_pageIndex >= m_maxPage)
            {
                m_pageIndex = m_maxPage - 1;
            }

            m_inputTimer = m_inputInterval;
        }
        else if (g_pad[0]->IsPress(enButtonLeft))
        {
            m_pageIndex--;

            if (m_pageIndex < 0)
            {
                m_pageIndex = 0;
            }

            m_inputTimer = m_inputInterval;
        }
    }

}

/// <summary>
/// 終了処理
/// </summary>
void Rule::UpdateExit()
{
    if (m_isEnding)
    {
        Game::SetState(GameState::Title);
        NewGO<Title>(0, "title");

		SEManager::Play(SE_return, false);

        m_isDead = true;
        return;
    }

    if (g_pad[0]->IsPress(enButtonB))
    {
        m_isEnding = true;

		SEManager::Play(SE_return, false);
        return;
    }
}
void Rule::Render(RenderContext& rc)
{

    // 現在ページだけ描画
        m_rules[m_pageIndex]->Draw(rc);

    // ボタン描画
    if (m_pageIndex > 0)
    {
        m_LeftButtonRender.Draw(rc);
       
    }

   
    if (m_pageIndex < m_maxPage - 1)
    {
        m_RightButtonRender.Draw(rc);
    }

}