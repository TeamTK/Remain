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
	//�i�C�t�̈ʒu
	if (*m_PlayerAnim == EPlayerAnim::eAnim_SetupGun)
	{
		//�\�����
		//SetTranselate(��, �O, ��)
		m_Model.SetTranselate(0.0f, 0.0f, 0.0f);
		m_Model.SetRotationDegree(0, 0, 0);
	}
}