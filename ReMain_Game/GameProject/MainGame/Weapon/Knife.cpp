#include "Knife.h"
#include "../enums.h"

Knife* g_pKnife;

Knife::Knife(int* anim, float* frame, bool* take, Matrix* m) :
	WeaponBase(anim, frame, take, m, "Knife", 0)
{
	m_Model.SetAsset("Knife");
}

Knife::~Knife()
{

}

void Knife::Update()
{
	//�i�C�t�̈ʒu
	if (*m_PlayerAnim == EPlayerAnim::eAnim_StealthAttack)
	{
			//�\�����
		//SetTranselate(��, �O, ��)
		m_Model.SetTranselate(0.0f, 0.01f, -0.03f);
		m_Model.SetRotationDegree(0, 180, 0);
	}
}