#include "WorldMatrixManager.h"
#include <list>

//Pimplイディオムでリスト保持
class WorldMatrixManager::ListPimpl
{
public:
	std::list<WorldMatrixInfo*> m_List;
	std::list<BoneMatrixFuncInfo*> m_BoneList;
};

//ワールド行列管理クラス
WorldMatrixManager::WorldMatrixManager()
{
	m_pListPimpl = new ListPimpl;
}

WorldMatrixManager::~WorldMatrixManager()
{
	m_pListPimpl->m_List.clear();
	m_pListPimpl->m_BoneList.clear();
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

void WorldMatrixManager::Add(BoneMatrixFuncInfo *pBoneMatrixFuncInfo)
{
	m_pListPimpl->m_BoneList.push_front(pBoneMatrixFuncInfo);
}

void WorldMatrixManager::Update()
{
	for (auto& i : m_pListPimpl->m_List)
	{
		i->func();
	}
}

void WorldMatrixManager::BoneUpdate()
{
	for (auto& i : m_pListPimpl->m_BoneList)
	{
		i->func();
	}
}

void WorldMatrixManager::Clear(WorldMatrixInfo *worldMatrixInfo)
{
	auto it = m_pListPimpl->m_List.begin();
	for (; it != m_pListPimpl->m_List.end(); it++)
	{
		if (*it == worldMatrixInfo)
		{
			it = m_pListPimpl->m_List.erase(it);
			break;
		}
	}
}

void WorldMatrixManager::Clear(BoneMatrixFuncInfo *pBoneMatrixFuncInfo)
{
	auto it = m_pListPimpl->m_BoneList.begin();
	for (; it != m_pListPimpl->m_BoneList.end(); it++)
	{
		if (*it == pBoneMatrixFuncInfo)
		{
			it = m_pListPimpl->m_BoneList.erase(it);
			break;
		}
	}
}

void WorldMatrixManager::AllClear()
{
	m_pListPimpl->m_List.clear();
	m_pListPimpl->m_BoneList.clear();
}