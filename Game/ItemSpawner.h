#pragma once
#include <vector>
#include "Item.h"

class ItemSpawner :public IGameObject
{
public:
	bool Start()override;

	void LateUpdate();	//遅延削除処理
	void SpawnNext();	//次のアイテム生成
	void StartThrow();	//現在のアイテムを投げる
	void Reset();		//初期化

	//現在のアイテム取得
	Item* GetCurrentItem()const { return m_currentItem; }

private:
	//出現させるアイテムリスト
	std::vector<ItemType>m_spawnList =
	{
		ItemType::ball,
		ItemType::egg,
	};

	//現在どのアイテムを出すか
	int m_index = 0;

	//今画面に存在しているアイテム
	Item* m_currentItem = nullptr;

	//削除待ちのアイテム(安全に削除するため)
	Item* m_pendingDeleteItem = nullptr;
};

