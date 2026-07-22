#include "stdafx.h"
#include "ItemSpawner.h"

bool ItemSpawner::Start()
{
	//出現インデックス初期化
	m_index = 0;

	//現在のアイテム話
	m_currentItem = nullptr;

	return true;
}

void ItemSpawner::SpawnNext()
{
	//今のアイテムがある場合→削除予約
	if (m_currentItem)
	{
		//LateUpdateで削除するため退避
		m_pendingDeleteItem = m_currentItem;

		//いったん非表示・非更新
		m_currentItem->SetActive(false);

		m_currentItem = nullptr;
	}

	//リストが空なら何もしない
	if (m_spawnList.empty())
	{
		return;
	}

	//新しいアイテムを生成
	m_currentItem = NewGO<Item>(0);

	//いったん非アクティブで初期化
	m_currentItem->SetActive(false);

	//種類を設定
	m_currentItem->Init(m_spawnList[m_index]);

	//放物運動の初期状態を準備
	m_currentItem->PrepareParabola();

	//有効化(ここで動き始める)
	m_currentItem->SetActive(true);

	//次のインデックス
	m_index++;

	//最後まで行ったら最初に戻る
	if (m_index >= m_spawnList.size())
	{
		m_index = 0;
	}
}

void ItemSpawner::StartThrow()
{
	//現在のアイテムがあるときだけ
	if (m_currentItem)
	{
		//アイテム側のMove()を呼ぶ
		m_currentItem->Move();
	}
}

void ItemSpawner::LateUpdate()
{
	//削除予約されているアイテムを削除
	if (m_pendingDeleteItem)
	{
		DeleteGO(m_pendingDeleteItem);
		m_pendingDeleteItem = nullptr;
	}
}

void ItemSpawner::Reset()
{
	//出現インデックスをリセット
	m_index = 0;

	//現在のアイテムを消す
	if (m_currentItem)
	{
		m_currentItem->ResetItem();
		m_currentItem->SetActive(false);
		DeleteGO(m_currentItem);
		m_currentItem = nullptr;
	}

	//削除待ちもリセット
	if (m_pendingDeleteItem)
	{
		m_pendingDeleteItem->SetActive(false);
		DeleteGO(m_pendingDeleteItem);
		m_pendingDeleteItem = nullptr;
	}
}
