#include "TracerouteObject.h"
#include "..\Mesh\StaticMesh.h"

TracerouteObject::TracerouteObject() :
	m_pStaticMesh(nullptr)
{
}

TracerouteObject::~TracerouteObject()
{
	m_pStaticMesh = nullptr;
}

void TracerouteObject::SetStaticMesh(StaticMesh *staticMesh)
{
	m_pStaticMesh = staticMesh;
}