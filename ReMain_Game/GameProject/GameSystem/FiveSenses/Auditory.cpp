#include "Auditory.h"
#include <list>
#include "../../GEKO/System/Math.h"

//聴覚探知
AuditorySense::AuditorySense() :
	m_IsStop(false),
	m_pDetectionRange(nullptr),
	m_pPos(nullptr)
{
	AuditoryManager::GetInstance()->AddAuditorySense(this);
}

AuditorySense::~AuditorySense()
{
	AuditoryManager::GetInstance()->ClaerAuditorySense(this);
}

void AuditorySense::SetFunc(std::function<void(int)> func)
{
	m_func = func;
}

void AuditorySense::SetDetectionRange(float *pRange)
{
	m_pDetectionRange = pRange;
}

void AuditorySense::SetPos(Vector3D *pPos)
{
	m_pPos = pPos;
}

void AuditorySense::Stop()
{
	m_IsStop = true;
}

void AuditorySense::Start()
{
	m_IsStop = false;
}

//聴覚探知対象
AuditoryObject::AuditoryObject()
{
	AuditoryManager::GetInstance()->AddAuditoryObject(this);
}

AuditoryObject::~AuditoryObject()
{
	AuditoryManager::GetInstance()->ClaerAuditoryObject(this);
}

void AuditoryObject::SetVolume(int *pVolume)
{
	m_pVolume = pVolume;
}

void AuditoryObject::SetPos(Vector3D *pPos)
{
	m_pPos = pPos;
}

//聴覚管理
class AuditoryManager::AuditoryPimpl
{
public:
	std::list<AuditorySense*> auditorySenseList;
	std::list<AuditoryObject*> auditoryObjectList;
};

AuditoryManager::AuditoryManager()
{
	m_pAuditoryPimpl = new AuditoryPimpl;
}

AuditoryManager::~AuditoryManager()
{
	m_pAuditoryPimpl->auditorySenseList.clear();
	m_pAuditoryPimpl->auditoryObjectList.clear();

	if (m_pAuditoryPimpl) delete m_pAuditoryPimpl;
}

AuditoryManager *AuditoryManager::GetInstance()
{
	static AuditoryManager auditoryManager;
	return &auditoryManager;
}

void AuditoryManager::AddAuditorySense(AuditorySense *pAuditorySense)
{
	m_pAuditoryPimpl->auditorySenseList.push_back(pAuditorySense);
}

void AuditoryManager::AddAuditoryObject(AuditoryObject *pAuditoryObject)
{
	m_pAuditoryPimpl->auditoryObjectList.push_back(pAuditoryObject);
}

void AuditoryManager::ClaerAuditorySense(AuditorySense *pAuditorySense)
{
	auto it = m_pAuditoryPimpl->auditorySenseList.begin();
	auto itEnd = m_pAuditoryPimpl->auditorySenseList.end();
	for (; it != itEnd; it++)
	{
		if (*it == pAuditorySense)
		{
			it = m_pAuditoryPimpl->auditorySenseList.erase(it);
			break;
		}
	}
}

void AuditoryManager::ClaerAuditoryObject(AuditoryObject *pAuditoryObject)
{
	auto it = m_pAuditoryPimpl->auditoryObjectList.begin();
	auto itEnd = m_pAuditoryPimpl->auditoryObjectList.end();
	for (; it != itEnd; it++)
	{
		if (*it == pAuditoryObject)
		{
			it = m_pAuditoryPimpl->auditoryObjectList.erase(it);
			break;
		}
	}
}

void AuditoryManager::AllClaer()
{
	m_pAuditoryPimpl->auditorySenseList.clear();
	m_pAuditoryPimpl->auditoryObjectList.clear();
}

void AuditoryManager::Update()
{
	Vector3D length;
	for (auto& i : m_pAuditoryPimpl->auditorySenseList)
	{
		if (i->m_IsStop) continue;
		for (auto& j : m_pAuditoryPimpl->auditoryObjectList)
		{
			//範囲計算
			length = *i->m_pPos - *j->m_pPos;
			if (length.LengthSq() < *i->m_pDetectionRange * *i->m_pDetectionRange)
			{
				i->m_func(*j->m_pVolume);
			}
		}
	}
}