#include "BaseCollider.h"

BaseCollider::BaseCollider() :
	m_MyId(NULL),
	m_TargetId(NULL),
	m_IsSeep(false)
{
}

BaseCollider::~BaseCollider()
{
}

bool BaseCollider::GetSleep() const
{
	return m_IsSeep;
}

unsigned int BaseCollider::GetMyID() const
{
	return m_MyId;
}

unsigned int BaseCollider::GetTargetID() const
{
	return m_TargetId;
}

void BaseCollider::SetID(unsigned int myId, unsigned int targetId)
{
	m_MyId = myId;
	m_TargetId = targetId;
}

void BaseCollider::Sleep()
{
	m_IsSeep = true;
}

void BaseCollider::Awake()
{
	m_IsSeep = false;
}