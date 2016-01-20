#include "Knife.h"
#include "../enums.h"

Knife* g_pKnife;

Knife::Knife(int* anim, float* frame, Matrix* m) :
	WeaponBase(anim, frame, m, "Knife", 0)
{
	m_Model.SetAsset("Knife");
}

Knife::~Knife()
{

}

void Knife::Update()
{
	//ナイフの位置
	if (*m_PlayerAnim == EPlayerAnim::eAnim_SetupGun)
	{
		//構え状態
		//SetTranselate(上, 前, 左)
		m_Model.SetTranselate(0.0f, 0.0f, 0.0f);
		m_Model.SetRotationDegree(0, 0, 0);
	}
}