#pragma once
#include <vector>
#include "Item.h"

class ItemSpawner :public IGameObject
{
public:
	bool Start()override;

	void LateUpdate();
	void SpawnNext();
	void StartThrow();
	void Reset();
	Item* GetCurrentItem()const { return m_currentItem; }

private:
	std::vector<ItemType>m_spawnList =
	{
		ItemType::ball,
		ItemType::egg,
	};

	int m_index = 0;

	Item* m_currentItem = nullptr;
	Item* m_pendingDeleteItem = nullptr;
};

