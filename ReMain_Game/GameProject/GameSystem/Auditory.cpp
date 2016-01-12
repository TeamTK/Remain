#include "Auditory.h"
#include <list>
#include "../GEKO/System/Math.h"

//íÆäoíTím
AuditorySense::AuditorySense() :
	m_IsStop(false),
	m_HearNum(0),
	m_DetectionRange(0.0f),
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

void AuditorySense::SetHearNum(int hearNum)
{
	m_HearNum = hearNum;
}

void AuditorySense::SetDetectionRange(float range)
{
	m_DetectionRange = range;
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

//íÆäoíTímëŒè€
AuditoryObject::AuditoryObject()
{
	AuditoryManager::GetInstance()->AddAuditoryObject(this);
}

AuditoryObject::~AuditoryObject()
{
	AuditoryManager::GetInstance()->ClaerAuditoryObject(this);
}

void AuditoryObject::SetVolume(int volume)
{
	m_Volume = volume;
}

void AuditoryObject::SetDetectionRange(float range)
{
	m_DetectionRange = range;
}

void AuditoryObject::SetPos(Vector3D *pPos)
{
	m_pPos = pPos;
}

//íÆäoä«óù
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
	for (auto& i : m_pAuditoryPimpl->auditorySenseList)
	{
		if (i->m_IsStop) continue;
		for (auto& j : m_pAuditoryPimpl->auditoryObjectList)
		{
			//íTímëŒè€ÇÃâπÇ™ï∑Ç±Ç¶ÇΩÇÁíTímë§ÇÃä÷êîÇåƒÇ‘
			if (j->m_Volume > i->m_HearNum)
			{
				i->m_func(j->m_Volume);
			}
		}
	}
}