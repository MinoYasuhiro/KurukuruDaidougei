#include "stdafx.h"
#include "ItemSpawner.h"

bool ItemSpawner::Start()
{
	m_index = 0;
	return true;
}

void ItemSpawner::SpawnNext()
{
	if (m_currentItem)
	{
		DeleteGO(m_currentItem);
		m_currentItem = nullptr;
	}

	if (m_spawnList.empty())
	{
		return;
	}

	m_currentItem = NewGO<Item>(0);

	m_currentItem->Init(m_spawnList[m_index]);

	m_currentItem->PrepareParabola();

	m_index++;

	if (m_index >= m_spawnList.size())
	{
		m_index = 0;
	}
}

void ItemSpawner::StartThrow()
{
	if (m_currentItem)
	{
		//m_currentItem->PrepareParabola();
		m_currentItem->Move();
	}
}

void ItemSpawner::Reset()
{
	m_index = 0;
	
	if (m_currentItem)
	{
		DeleteGO(m_currentItem);
		m_currentItem = nullptr;
	}
}
