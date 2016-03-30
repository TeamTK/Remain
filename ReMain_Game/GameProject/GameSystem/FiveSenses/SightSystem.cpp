#include "SightSystem.h"
#include "..\..\GEKO\GEKO.h"
#include <list>

EnemySight::EnemySight() :
	m_isSleep(false),
	m_isObstacle(true),
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

void EnemySight::Regist(SightData *pSightdata, std::function<void(const Vector3D *)> func)
{
	assert(m_Func == nullptr && "EnemySight�͂��łɓo�^����Ă��܂�");
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
	std::cout << "HITSIGHT" << "\n";

	m_isObstacle = true;
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
	m_pPos = pPos;
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
	float angle = 0;
	float dot = 0;
	Vector3D plyaerVec;
	Vector3D EnemyVec;

	//�S�Ă̓G�̎��E����
	auto it = m_pSightPimpl->EnemyList.begin();
	auto itEnd = m_pSightPimpl->EnemyList.end();
	for (; it != itEnd; it++)
	{
		if ((*it)->m_isSleep) continue;
		if (m_pPlayerSightInfo->m_isSleep) continue;
		
		//���E�v�Z
		EnemyVec = *(*it)->m_pSightData->pSightVec;
		plyaerVec = *m_pPlayerSightInfo->m_pPos - *(*it)->m_pSightData->pSightPos;
		dot = Vector3D::Dot(plyaerVec.GetNormalize(), EnemyVec.GetNormalize());
		angle = Math::ChangeToDegree(acos(dot));

		//���E�p�x
		if (angle < (*it)->m_pSightData->angle)
		{
			//����
			if (plyaerVec.LengthSq() < (*it)->m_pSightData->distance * (*it)->m_pSightData->distance)
			{
				Result_Porygon pory;
				if (!ColliderManager::GetInstance()->HitCheckStaticMesh_Line(
					&pory, (*it)->m_pSightData->pSightPos, 
					m_pPlayerSightInfo->m_pPos, eHITID3 | eHITID4 | eHITID5 | eHITID7))
				{
					(*it)->m_Func(m_pPlayerSightInfo->m_pPos);
				}
			}
		}
	}
}