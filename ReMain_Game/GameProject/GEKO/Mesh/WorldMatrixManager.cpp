#include "WorldMatrixManager.h"
#include <list>

//Pimplイディオムでリスト保持
class WorldMatrixManager::ListPimpl
{
public:
	std::list<WorldMatrixInfo*> m_List;
};

//ワールド行列管理クラス
WorldMatrixManager::WorldMatrixManager()
{
	m_pListPimpl = new ListPimpl;
}

WorldMatrixManager::~WorldMatrixManager()
{
	m_pListPimpl->m_List.clear();
	delete m_pListPimpl;
}

WorldMatrixManager *WorldMatrixManager::GetInstance()
{
	static WorldMatrixManager WorldMatrixManager;
	return &WorldMatrixManager;
}

void WorldMatrixManager::Add(WorldMatrixInfo *worldMatrixInfo)
{
	m_pListPimpl->m_List.push_front(worldMatrixInfo);
}

void WorldMatrixManager::Update()
{
	for (auto& i : m_pListPimpl->m_List)
	{
		i->func();
	}
}

void WorldMatrixManager::Clear(WorldMatrixInfo *worldMatrixInfo)
{
	auto it = m_pListPimpl->m_List.begin();
	for (; it != m_pListPimpl->m_List.end(); it++)
	{
		//ワールド行列がなかったらリストから除外
		if (*it == worldMatrixInfo)
		{
			it = m_pListPimpl->m_List.erase(it);
			break;
		}
	}
}

void WorldMatrixManager::AllClear()
{
	m_pListPimpl->m_List.clear();
}