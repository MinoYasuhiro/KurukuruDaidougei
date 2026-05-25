#include "stdafx.h"
#include "CoinEffect.h"

bool CoinEffect::Start()
{
	srand((unsigned int)time(nullptr));

	m_nextIndex = 0;

	for (int i = 0;i < COIN_MAX;i++)
	{
		m_coin[i].Init("Assets/sprite/Coin.dds", 60.0f, 60.0f);
		m_coins[i].active = false;
	}

	return true;
}


void CoinEffect::Update()
{
	float dt = 1.0f / 60.0f;

	for (int i = 0;i < COIN_MAX;i++)
	{
		if (!m_coins[i].active)continue;

		m_coins[i].speed.y -= 120.0f * dt;
		m_coins[i].position += m_coins[i].speed * dt;

		if (m_coins[i].position.y < -600.0f)
		{
			m_coins[i].active = false;
		}
	}
}

void CoinEffect::Play()
{
	for (int i = 0;i < COIN_MAX;i++)
	{
		int j = m_nextIndex;

		m_coins[j].active = true;

		float baseX = (rand() % 2 == 0) ? -1600.0f : 1600.0f;
		float x = baseX + (rand() % 300 - 150);
		float y = (float)(rand() % 600 + 100);

		m_coins[j].position = { x,y,0.0f };

		Vector3 targetPos = { (float)(rand() % 40 - 20),(float)(rand() % 30 - 15),0.0f };

		Vector3 dir = targetPos - m_coins[j].position;

		if (dir.Length() > 0.001f)
		{
			dir.Normalize();
		}

		m_coins[j].speed = dir * 500.0f;
		m_coins[j].speed.y += 60.0f;

		m_nextIndex++;
		if (m_nextIndex >= COIN_MAX)
		{
			m_nextIndex = 0;
		}
	}
}

void CoinEffect::Render(RenderContext& rc)
{
	for (int i = 0;i < COIN_MAX;i++)
	{
		if (!m_coins[i].active)continue;

		m_coin[i].SetPosition(m_coins[i].position);
		m_coin[i].Update();
		m_coin[i].Draw(rc);
	}
}