#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "SoundOption.h"
#include "BGMManager.h"
#include "BGM.h"
#include "TitleMenu.h"
#include "Operation.h"
#include "Rule.h"
#include "Loading.h"

namespace TITLE {

	const Vector3 POSITION = { 0.0f,0.0f,0.0f };
}

bool Title::Start()
{
	m_spriteRender.Init("Assets/Sprite/Title.DDS", 1920, 1080);
	m_spriteRender.SetPosition({ TITLE::POSITION });

	m_titleMenu = NewGO<TitleMenu>(0, "titleMenu");

	m_BGM = FindGO<BGMManager>("bgmManager");

	if (m_BGM == nullptr)
	{
		m_BGM = NewGO <BGMManager>(0, "bgmManager");
		m_BGM->Init();
	}
	
	m_BGM->Play(BGM_Title);

	return true;
}

void Title::Update()
{
	m_spriteRender.Update();

	if (!m_titleMenu) return;

	switch (m_titleMenu->GetResult())
	{
	     case MenuResult::GameStart:
	     {
			 if (m_BGM)
			 {
				 m_BGM->Stop();
			 }

		    NewGO<Loading>(0, "loading");


		    DeleteGO(m_titleMenu);
		    m_titleMenu = nullptr;

		    DeleteGO(this);
		    return;
	     }
	   

	    case MenuResult::SoundOption:
	    {
		   Game::SetPrevState(GameState::Title);
		   Game::SetState(GameState::SoundTest);

		   auto soundOption=NewGO<SoundOption>(0, "soundOption");

		   soundOption->SetTransitionSource(TransitionSource::FromTitle);

		   DeleteGO(m_titleMenu);
		   m_titleMenu = nullptr;
	   	   DeleteGO(this);
		   return;
	    }

	    case MenuResult::Operation:
	    {
	   	   Game::SetPrevState(GameState::Title);
		   NewGO<Operation>(0, "operation");

		   DeleteGO(m_titleMenu);
		   m_titleMenu = nullptr;
		   DeleteGO(this);
		   return;
	    }

		case MenuResult::Rule:
		{
			Game::SetPrevState(GameState::Title);
			NewGO<Rule>(0, "rule");

			DeleteGO(m_titleMenu);
			m_titleMenu = nullptr;
			m_isDead = true;
			return;
		}
    }
}

void Title::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}