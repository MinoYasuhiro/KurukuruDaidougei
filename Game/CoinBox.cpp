#include "stdafx.h"
#include "CoinBox.h"
#include "Game.h"

CoinBox::CoinBox()
{

}

CoinBox::~CoinBox()
{

}

bool CoinBox::Start()
{
	m_coinBoxRender.Init("Assets/modelData/coinHolder.tkm");
	m_coinBoxRender.SetScale({ 1.5f,1.5f,1.5f });
	m_smallCoinRender.Init("Assets/modelData/smallCoin.tkm");
	m_smallCoinRender.SetScale({ 1.5f,1.5f,1.5f });
	m_mediumCoinRender.Init("Assets/modelData/mediumCoin.tkm");
	m_mediumCoinRender.SetScale({ 1.5f,1.5f,1.5f });
	m_largeCoinRender.Init("Assets/modelData/largeCoin.tkm");
	m_largeCoinRender.SetScale({ 1.5f,1.5f,1.5f });
	m_extraLargeCoinRender.Init("Assets/modelData/extraLargeCoin.tkm");
	m_extraLargeCoinRender.SetScale({ 1.5f,1.5f,1.5f });
	m_maxCoinRender.Init("Assets/modelData/maxCoin.tkm");
	m_maxCoinRender.SetScale({ 1.5f,1.5f,1.5f });

	return true;
}

void CoinBox::Update()
{
	if (g_gameTime == nullptr)return;

	m_coinBoxRender.SetPosition(m_position);
	m_coinBoxRender.Update();

	m_smallCoinRender.SetPosition(m_position);
	m_smallCoinRender.Update();

	m_mediumCoinRender.SetPosition(m_position);
	m_mediumCoinRender.Update();

	m_largeCoinRender.SetPosition(m_position);
	m_largeCoinRender.Update();

	m_extraLargeCoinRender.SetPosition(m_position);
	m_extraLargeCoinRender.Update();

	m_maxCoinRender.SetPosition(m_position);
	m_maxCoinRender.Update();

	if (Game::GetState() != GameState::Playing)return;

	float deltaTime = g_gameTime->GetFrameDeltaTime();

	m_testTime += deltaTime;

	if (m_testTime > 10.0f)
	{
		m_testTime = 0.0f;

		switch (m_coinLevel)
		{
		case CoinLevel::Empty:
			m_coinLevel = CoinLevel::Small;
			break;
		case CoinLevel::Small:
			m_coinLevel = CoinLevel::Medium;
			break;
		case CoinLevel::Medium:
			m_coinLevel = CoinLevel::Large;
			break;
		case CoinLevel::Large:
			m_coinLevel = CoinLevel::ExtraLarge;
			break;
		case CoinLevel::ExtraLarge:
			m_coinLevel = CoinLevel::Max;
			break;
		case CoinLevel::Max:
			break;
		default:
			break;
		}
	}
}

void CoinBox::Render(RenderContext& rc)
{
	switch (m_coinLevel)
	{
	case CoinLevel::Empty:
		m_coinBoxRender.Draw(rc);
		break;
	case CoinLevel::Small:
		m_smallCoinRender.Draw(rc);
		break;
	case CoinLevel::Medium:
		m_mediumCoinRender.Draw(rc);
		break;
	case CoinLevel::Large:
		m_largeCoinRender.Draw(rc);
		break;
	case CoinLevel::ExtraLarge:
		m_extraLargeCoinRender.Draw(rc);
		break;
	case CoinLevel::Max:
		m_maxCoinRender.Draw(rc);
		break;
	default:
		break;
	}
}