#include "Gravity.h"
#include "..\GEKO\System\Math.h"

Gravity::Gravity(Vector3D *pPos, bool *pIsKill) :
	Task("Gravity", 0),
	m_pPos(pPos),
	m_pIsKill(pIsKill)
{
}

Gravity::~Gravity()
{
}

void Gravity::Update()
{
	if (*m_pIsKill)
	{
		SetKill();
		return;
	}

	m_pPos->y -= 0.03f;
}