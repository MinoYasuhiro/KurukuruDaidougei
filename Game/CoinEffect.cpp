#include "stdafx.h"
#include "CoinEffect.h"

bool CoinEffect::Start()
{
	//乱数の初期化(毎回違う動きになるように)
	srand((unsigned int)time(nullptr));

	m_nextIndex = 0;

	for (int i = 0;i < COIN_MAX;i++)
	{
		m_coin[i].Init("Assets/sprite/Coin.dds", 60.0f, 60.0f);

		//初期状態は「非表示」
		m_coins[i].active = false;
	}

	return true;
}


void CoinEffect::Update()
{
	//1フレームの時間(60FPS想定)
	float dt = 1.0f / 60.0f;

	for (int i = 0;i < COIN_MAX;i++)
	{
		//非アクティブなら処理しない
		if (!m_coins[i].active)continue;

		//重力処理(下に引っ張る)
		m_coins[i].speed.y -= 120.0f * dt;

		//位置更新(速度を使って移動)
		m_coins[i].position += m_coins[i].speed * dt;

		//一定以下に落ちたら消す(画面外)
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
		//使用するコインのインデックス
		int j = m_nextIndex;

		//コインを有効化(表示)
		m_coins[j].active = true;

		//左右どちらかからスタート
		float baseX = (rand() % 2 == 0) ? -1600.0f : 1600.0f;

		//ランダムでばらつきを付ける
		float x = baseX + (rand() % 300 - 150);
		float y = (float)(rand() % 600 + 100);

		//初期位置設定
		m_coins[j].position = { x,y,0.0f };

		//画面中央付近を目標地点にする
		Vector3 targetPos = { (float)(rand() % 40 - 20),(float)(rand() % 30 - 15),0.0f };

		//進行方向ベクトル=目標-現在位置
		Vector3 dir = targetPos - m_coins[j].position;

		//長さを１にする(正規化)
		if (dir.Length() > 0.001f)
		{
			dir.Normalize();
		}

		//初速設定(勢いよく飛ばす)
		m_coins[j].speed = dir * 500.0f;

		//少し上に持ち上げる
		m_coins[j].speed.y += 60.0f;

		//次のコインへ
		m_nextIndex++;

		//最大値を超えたら先頭に戻る(使いまわし)
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
		//アクティブなものだけ描画
		if (!m_coins[i].active)continue;

		//スプライトに位置を反映
		m_coin[i].SetPosition(m_coins[i].position);

		//内部処理更新
		m_coin[i].Update();

		//描画
		m_coin[i].Draw(rc);
	}
}