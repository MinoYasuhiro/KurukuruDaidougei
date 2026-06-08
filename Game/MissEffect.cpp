#include "stdafx.h"
#include "MissEffect.h"

bool MissEffect::Start()
{
	//乱数初期化
	srand((unsigned int)time(nullptr));

	//アクティブリストの容量をあらかじめ確保
	m_activeList.reserve(MISS_MAX);

	m_nextIndex = 0;

	//初期化
	for (int i = 0;i < MISS_MAX;i++)
	{
		//種類を分ける
		int type = i % Type_NUM;

		//スプライト初期化
		switch (type)
		{
		case Bucket:
			m_sprite[i].Init("Assets/sprite/Bucket.dds", 100.0f, 100.0f);
			break;
		case Basket:
			m_sprite[i].Init("Assets/sprite/Basket.dds", 100.0f, 100.0f);
			break;
		case Colander:
			m_sprite[i].Init("Assets/sprite/Colander.dds", 100.0f, 100.0f);
			break;
		case Fan:
			m_sprite[i].Init("Assets/sprite/Fan.dds", 100.0f, 100.0f);
			break;
		case SakeBottle:
			m_sprite[i].Init("Assets/sprite/SakeBottle.dds", 100.0f, 100.0f);
			break;
		case Stone:
			m_sprite[i].Init("Assets/sprite/Stone.dds", 100.0f, 100.0f);
			break;
		}

		//データ初期化
		m_objects[i].active = false;
		m_objects[i].rotation = 0.0f;
		m_objects[i].type = type;
	}

	return true;
}

void MissEffect::Update()
{
	float dt = 1.0f / 60.0f;

	for (int i = 0;i < m_activeList.size();i++)
	{
		int j = m_activeList[i];

		//非アクティブor不正なら高速削除
		if (j < 0 || j >= MISS_MAX || !m_objects[j].active)
		{
			//最後と入れ替え
			m_activeList[i] = m_activeList.back();
			//削除
			m_activeList.pop_back();
			i--;
			continue;
		}

		float gravityScale = 1.8f;

		m_objects[j].speed.y -= 300.0f * gravityScale * dt;

		//移動
		m_objects[j].position += m_objects[j].speed * dt;

		//回転
		m_objects[j].rotation += 90.0f * dt;

		//画面外で消す
		if (m_objects[j].position.y < -800.0f)
		{
			m_objects[j].active = false;
		}
	}
}

void MissEffect::Play(int failCount)
{
	//前回のエフェクトをリセット
	m_activeList.clear();

	//全オブジェクトを非アクティブにする
	for (int i = 0;i < MISS_MAX;i++)
	{
		m_objects[i].active = false;
	}

	//投げる数
	int spawnNum = failCount * 10;

	if (spawnNum > MISS_MAX)
	{
		spawnNum = MISS_MAX;
	}

	for (int i = 0;i < spawnNum;i++)
	{
		int j = m_nextIndex;

		//新しく使う場合のみリストに追加
		if (!m_objects[j].active)
		{
			m_activeList.push_back(j);
		}

		m_objects[j].active = true;

		//左右どちらかから出す
		float startX = (rand() % 2 == 0) ? -1600.0f : 1600.0f;
		float startY = (float)(rand() % 400 - 200);

		m_objects[j].position = { startX,startY,0.0f };

		//ランダムな目標地点
		Vector3 target = {
		(float)(rand() % 600 - 300),
		(float)(rand() % 200 - 100),
		0.0f
		};

		//進行方向
		Vector3 dir = target - m_objects[j].position;

		if (dir.Length() > 0.001f)
		{
			dir.Normalize();
		}

		//速度
		float speed = 500.0f;
		float speedScale = 1.8f;
		m_objects[j].speed = dir * speed;

		//横方向のばらつき
		m_objects[j].speed.x += (rand() % 200 - 100);

		//上に持ち上げる
		m_objects[j].speed.y += (rand() % 40 + 20);

		//軽くばらけさせる
		m_objects[j].speed.y += 60.0f;

		m_objects[j].speed *= speedScale;

		//次のインデックスへ
		m_nextIndex++;
		if (m_nextIndex >= MISS_MAX)
		{
			m_nextIndex = 0;
		}
	}
}

void MissEffect::Reset()
{
	for (int i = 0;i < MISS_MAX;i++)
	{
		m_objects[i].active = false;
	}

	m_nextIndex = 0;
	m_activeList.clear();
}

void MissEffect::Render(RenderContext& rc)
{
	for (int i = 0;i < m_activeList.size();i++)
	{
		int j = m_activeList[i];

		if (j < 0 || j >= MISS_MAX)continue;
		if (!m_objects[j].active)continue;

		//位置設定
		m_sprite[j].SetPosition(m_objects[j].position);

		//回転設定
		Quaternion rot;
		rot.SetRotationDegZ(m_objects[j].rotation);
		m_sprite[j].SetRotation(rot);

		m_sprite[j].Update();
		m_sprite[j].Draw(rc);
	}
}