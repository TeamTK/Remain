#include "SightSystem.h"
#include <list>
#include <assert.h>

EnemySight::EnemySight() :
	m_isSleep(false),
	m_isObstacle(false),
	m_pSightData(nullptr)
{
}

EnemySight::~EnemySight()
{
	if (m_Func != nullptr)
	{
		Release();
	}
}

bool EnemySight::GetSleep() const
{
	return m_isSleep;
}

void EnemySight::Regist(SightData *pSightdata, std::function<void()> func)
{
	assert(m_Func != nullptr && "EnemySight‚Í‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚Ü‚·");
	m_pSightData = pSightdata;
	m_Func = func;
	SightManager::GetInstance()->AddEnemy(this);
}

void EnemySight::Release()
{
	m_isSleep = false;
	m_isObstacle = false;
	m_pSightData = nullptr;
	m_Func = nullptr;
	SightManager::GetInstance()->Clear(this);
}

void EnemySight::Sleep()
{
	m_isSleep = true;
}

void EnemySight::Awake()
{
	m_isSleep = false;
}

void EnemySight::HitSight(Result_Porygon &data)
{
	m_isObstacle = true;
	m_HitSight.Sleep();
}

PlayerSightInfo::PlayerSightInfo() :
	m_isSleep(false)
{
	SightManager::GetInstance()->AddPlayer(this);
}

PlayerSightInfo::~PlayerSightInfo()
{
}

void PlayerSightInfo::SetPos(Vector3D *pPos)
{
	pPos = pPos;
}

void PlayerSightInfo::Sleep()
{
	m_isSleep = true;
}

void PlayerSightInfo::Awake()
{
	m_isSleep = false;
}

class SightManager::SightPimpl
{
public:
	std::list<EnemySight*> EnemyList;
};

SightManager::SightManager()
{
	m_pSightPimpl = new SightPimpl;
}

SightManager::~SightManager()
{
	if (m_pSightPimpl != nullptr)
	{
		delete m_pSightPimpl;
		m_pSightPimpl = nullptr;
	}
}

SightManager *SightManager::GetInstance()
{
	static SightManager sightManager;
	return &sightManager;
}

void SightManager::AddEnemy(EnemySight *pEnemySight)
{
	m_pSightPimpl->EnemyList.emplace_back(pEnemySight);
}

void SightManager::AddPlayer(PlayerSightInfo *pPlayerSightInfo)
{
	m_pPlayerSightInfo = pPlayerSightInfo;
}

void SightManager::Clear(EnemySight *pEnemySight)
{
	auto it = m_pSightPimpl->EnemyList.begin();
	auto itEnd = m_pSightPimpl->EnemyList.end();
	for (; it != itEnd; it++)
	{
		if (*it == pEnemySight)
		{
			it = m_pSightPimpl->EnemyList.erase(it);
			break;
		}
	}
}

void SightManager::AllClear()
{
	auto it = m_pSightPimpl->EnemyList.begin();
	auto itEnd = m_pSightPimpl->EnemyList.end();
	for (; it != itEnd; )
	{
		it = m_pSightPimpl->EnemyList.erase(it);
	}
	m_pSightPimpl->EnemyList.clear();
}

void SightManager::Update()
{
	auto it = m_pSightPimpl->EnemyList.begin();
	auto itEnd = m_pSightPimpl->EnemyList.end();
	for (; it != itEnd; it++)
	{
		if ((*it)->m_isSleep) continue;
		if (m_pPlayerSightInfo->m_isSleep) continue;
		
		float dot = Vector3D::Dot(*(*it)->m_pSightData->pSightVec, *m_pPlayerSightInfo->pPos);
		float angle = acos(dot);

		std::cout << (*it)->m_pSightData->angle << "\n";

		if ((*it)->m_pSightData->angle < angle)
		{
			std::cout << "aaaaaaaaaaaaaaaaaaaaaa" << "\n";
		}
	}
}