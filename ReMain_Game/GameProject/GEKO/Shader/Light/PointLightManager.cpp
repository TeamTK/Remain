#include "PointLightManager.h"
#include "../ShaderInfo.h"
#include <vector>
#include <assert.h>

struct PointLightManager::LightPimpl
{
	int forwardLightNum;
	int deferredLightNum;
	std::vector<PointLight*> forwardList;
	std::vector<PointLight*> deferredList;
};

PointLightManager::PointLightManager()
{
	m_pLightPimpl = new LightPimpl;
	m_pLightPimpl->forwardLightNum = 0;
	m_pLightPimpl->deferredLightNum = 0;
}

PointLightManager::~PointLightManager()
{
	Release();
	delete m_pLightPimpl;
}

PointLightManager* PointLightManager::GetInstance()
{
	static PointLightManager pointLightManager;
	return &pointLightManager;
}

const int PointLightManager::GetForwardLightNum() const
{
	return m_pLightPimpl->forwardLightNum;
}

const int PointLightManager::GetDeferredLightNum() const
{
	return m_pLightPimpl->deferredLightNum;
}

const PointLight* PointLightManager::GetForwardLight(int arrayNum) const
{
	return m_pLightPimpl->forwardList[arrayNum];
}

const PointLight* PointLightManager::GetDeferreLight(int arrayNum) const
{
	return m_pLightPimpl->deferredList[arrayNum];
}

void PointLightManager::ForwardAdd(PointLight *pPointLight)
{
	m_pLightPimpl->forwardLightNum++;
	assert(m_pLightPimpl->forwardLightNum < FORWARD_LIGHT_MAX + 1 && "Forwardのポイントライトの個数制限です");
	m_pLightPimpl->forwardList.emplace_back(pPointLight);
}

void PointLightManager::DeferredAdd(PointLight *pPointLight)
{
	m_pLightPimpl->deferredLightNum++;
	assert(m_pLightPimpl->deferredLightNum != DEFERRED_LIGHT_MAX && "Deferredのポイントライトの個数制限です");
	m_pLightPimpl->deferredList.emplace_back(pPointLight);
}

void PointLightManager::ForwardClear(PointLight *pPointLight)
{
	auto it = m_pLightPimpl->forwardList.begin();
	auto itEnd = m_pLightPimpl->forwardList.end();

	for (; it != itEnd; it++)
	{
		if (*it == pPointLight)
		{
			it = m_pLightPimpl->forwardList.erase(it);
			m_pLightPimpl->forwardLightNum--;
			break;
		}
	}
}

void PointLightManager::DeferredClear(PointLight *pPointLight)
{
	auto it = m_pLightPimpl->deferredList.begin();
	auto itEnd = m_pLightPimpl->deferredList.end();

	for (; it != itEnd; it++)
	{
		if (*it == pPointLight)
		{
			it = m_pLightPimpl->deferredList.erase(it);
			m_pLightPimpl->deferredLightNum--;
			break;
		}
	}
}

void PointLightManager::Release()
{
	m_pLightPimpl->forwardList.clear();
	m_pLightPimpl->forwardList.shrink_to_fit();

	m_pLightPimpl->deferredList.clear();
	m_pLightPimpl->deferredList.shrink_to_fit();
}