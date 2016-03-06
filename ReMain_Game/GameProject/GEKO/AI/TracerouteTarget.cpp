#include "TracerouteTarget.h"
#include "TracerouteManager.h"

TracerouteTarget::TracerouteTarget() :
	m_pPosition(nullptr)
{
	TracerouteManager::AddTarget(this);
}

TracerouteTarget::~TracerouteTarget()
{
	TracerouteManager::ClearTarget(this);
}

void TracerouteTarget::SetName(std::string name)
{
	m_Name = name;
}

void TracerouteTarget::SetPosition(Vector3D *pPosition)
{
	m_pPosition = pPosition; 
}