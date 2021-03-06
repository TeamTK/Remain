#include "Knife.h"
#include "../Player/PlayerInfo.h"

Knife::Knife(int* anim, float* frame, bool* take, Matrix* m) :
	WeaponBase(anim, frame, take, m, "Knife", 0)
{
	m_Model.SetRenderingRegister(true, 1, 0);
	m_Model.SetAsset("Knife");
	m_pMatirx = m;
}

Knife::~Knife()
{

}

void Knife::Update()
{
	//ナイフの位置
	if (*m_PlayerAnim == EPlayerAnim::eAnim_StealthAttack)
	{
		//構え状態
		//SetTranselate(上, 前, 左)
		m_Model.SetTranselate(0.0f, 0.01f, -0.03f);
		m_Model.SetRotationDegree(0, 180, 0);
	}

	m_Model.SetParentMatirx(*m_pMatirx);
}