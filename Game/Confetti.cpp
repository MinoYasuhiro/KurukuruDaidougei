#include "stdafx.h"
#include "Confetti.h"

bool Confetti::Start()
{
	//乱数の初期化
	srand((unsigned int)time(nullptr));

	//紙吹雪のインデックス初期化
	m_nextIndex = 0;

	//全ての紙吹雪インスタンス初期化
	for (int i = 0;i < CONFETTI_MAX;i++)
	{
		//種類をループで分ける
		int type = i % TYPE_NUM;

		//タイプ毎にスプライトを初期化
		switch (type)
		{
		case 0:
			m_confetti[i].Init("Assets/sprite/ConfettiRed.dds", 75.0f, 75.0f);
			break;
		case 1:
			m_confetti[i].Init("Assets/sprite/ConfettiGold.dds", 75.0f, 75.0f);
			break;
		case 2:
			m_confetti[i].Init("Assets/sprite/ConfettiWhite.dds", 75.0f, 75.0f);
			break;
		case 3:
			m_confetti[i].Init("Assets/sprite/ConfettiPurple.dds", 75.0f, 75.0f);
			break;
		}

		//最初はすべて非アクティブ(表示しない)
		m_confettis[i].active = false;
	}

	return true;
}

void Confetti::Update()
{
	//1フレームの時間(60FPS想定)
	float dt = 1.0f / 60.0f;

	for (int i = 0;i < CONFETTI_MAX;i++)
	{
		//非アクティブなら処理しない
		if (!m_confettis[i].active)continue;

		//重力処理
		m_confettis[i].speed.y -= 60.0f * dt;

		//位置更新
		m_confettis[i].position += m_confettis[i].speed * dt;

		//横揺れ(sin波でふわふわ感を出す)
		m_confettis[i].position.x += sinf(m_confettis[i].position.y * 0.05f) * 2.0f;

		//回転更新
		m_confettis[i].rotation += m_confettis[i].rotSpeed * dt;

		//画面外に出たら非アクティブ化(再利用)
		if (m_confettis[i].position.y < -600.0f)
		{
			m_confettis[i].active = false;
		}
	}
}

void Confetti::Play()
{
	for (int i = 0;i < CONFETTI_MAX;i++)
	{
		//次に使う紙吹雪のインデックス
		int j = m_nextIndex;

		//紙吹雪を有効化
		m_confettis[j].active = true;

		//ランダムな初期位置(画面上部あたり)
		float x = (float)(rand() % 1800 - 900);
		float y = (float)(rand() % 1500);

		m_confettis[j].position = { x,y,0.0f };

		//落下速度をランダムで設定
		float speed = (rand() % 200 + 100);

		//X方向はランダムな揺れ
		m_confettis[j].speed.x = (rand() % 100 - 50);
		//Y方向は下向き
		m_confettis[j].speed.y = -speed;

		//初期回転
		m_confettis[j].rotation = 0.0f;
		//回転スピード
		m_confettis[j].rotSpeed = (rand() % 60 - 30);

		//種類設定
		m_confettis[j].type = rand() % TYPE_NUM;

		//次のインデックス
		m_nextIndex++;

		//配列の最後まで行ったら先頭に戻る
		if (m_nextIndex >= CONFETTI_MAX)
		{
			m_nextIndex = 0;
		}
	}
}

void Confetti::Render(RenderContext& rc)
{
	for (int i = 0;i < CONFETTI_MAX;i++)
	{
		//非アクティブは描画しない
		if (!m_confettis[i].active)continue;

		//位置をスプライトに反映
		m_confetti[i].SetPosition(m_confettis[i].position);

		//回転設定(Z軸回転)
		Quaternion rot;
		rot.SetRotationDegZ(m_confettis[i].rotation);
		m_confetti[i].SetRotation(rot);

		m_confetti[i].Update();
		m_confetti[i].Draw(rc);
	}
}