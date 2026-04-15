#include "stdafx.h"
#include "CoinBox.h"

CoinBox::CoinBox()
{

}

CoinBox::~CoinBox()
{

}

bool CoinBox::Start()
{
	m_coinBoxRender.Init("Assets/modelData/coinHolder.tkm");
	m_smallCoinRender.Init("Assets/modelData/smallCoin.tkm");
	m_mediumCoinRender.Init("Assets/modelData/mediumCoin.tkm");
	m_largeCoinRender.Init("Assets/modelData/largeCoin.tkm");
	m_extraLargeCoinRender.Init("Assets/modelData/extraLargeCoin.tkm");
	m_maxCoinRender.Init("Assets/modelData/maxCoin.tkm");
	return true;
}

void CoinBox::Update()
{
	float deltaTime =g_gameTime->GetFrameDeltaTime();
	m_testTime += deltaTime;
	if (m_testTime > 2.0f)
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
		default:
			break;
		}
	}
	m_coinBoxRender.SetPosition(m_position);
	m_coinBoxRender.Update();
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