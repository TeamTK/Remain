#include "RenderManager.h"
#include "StaticMesh\StaticMesh.h"
#include "DynamicMesh\DynamicMesh.h"
#include <list>
/*
//Pimplイディオムでリスト保持
class RenderManager::ListPimpl
{
public:
	std::list<StaticMesh*> staticMeshList;
	std::list<DynamicMesh*> dynamicMeshList;
};

RenderManager::RenderManager()
{
	m_pListPimpl = new ListPimpl;
}

RenderManager::~RenderManager()
{
	AllClear();
	delete m_pListPimpl;
}

RenderManager *RenderManager::GetInstance()
{
	static RenderManager renderManager;
	return &renderManager;
}

void RenderManager::Add(StaticMesh *pStaticMesh)
{
	m_pListPimpl->staticMeshList.push_front(pStaticMesh);
}

void RenderManager::Add(DynamicMesh *pDynamicMesh)
{
	m_pListPimpl->dynamicMeshList.push_front(pDynamicMesh);
}

void RenderManager::Update()
{
	for (auto& i : m_pListPimpl->staticMeshList)
	{
		
	}

	for (auto& i : m_pListPimpl->dynamicMeshList)
	{

	}
}

void RenderManager::Clear(StaticMesh *pStaticMesh)
{
	auto it = m_pListPimpl->staticMeshList.begin();
	auto itEnd = m_pListPimpl->staticMeshList.end();
	for (; it != itEnd; it++)
	{
		if (*it == pStaticMesh)
		{
			it = m_pListPimpl->staticMeshList.erase(it);
			break;
		}
	}
}

void RenderManager::Clear(DynamicMesh *pDynamicMesh)
{
	auto it = m_pListPimpl->dynamicMeshList.begin();
	auto itEnd = m_pListPimpl->dynamicMeshList.end();
	for (; it != itEnd; it++)
	{
		if (*it == pDynamicMesh)
		{
			it = m_pListPimpl->dynamicMeshList.erase(it);
			break;
		}
	}
}

void RenderManager::AllClear()
{
	m_pListPimpl->staticMeshList.clear();
	m_pListPimpl->dynamicMeshList.clear();
}

*/