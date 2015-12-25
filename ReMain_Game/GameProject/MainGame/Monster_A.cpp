#include "Monster_A.h"

Monster_A::Monster_A(Vector3D pos, Vector3D rot) :
	Enemy(pos, rot, "Monster_A")
{
	//�e���ʂ̃J�v�Z���̏��
	BoneCapsule.emplace_back(0.4f, 25, 4);	  //��
	BoneCapsule.emplace_back(0.6f, 2, 4);   //����
	BoneCapsule.emplace_back(0.5f, 6, 8);   //���r
	BoneCapsule.emplace_back(0.5f, 16, 18); //�E�r
	BoneCapsule.emplace_back(0.5f, 26, 28); //����
	BoneCapsule.emplace_back(0.5f, 30, 32); //�E��

	//���i�e�j�Ƃ̔���p
	m_pCollider = new Collider[BoneCapsule.size()];
	m_pCapsule = new CapsuleInfo[BoneCapsule.size()];

	unsigned int bornNum = BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		m_pCollider[i].Regist_C_vs_S(&m_pCapsule[i].segment.start, &m_pCapsule[i].segment.end,
			&m_pCapsule[i].radius, REGIST_FUNC(Enemy::HitBullet));
		m_pCollider[i].SetID(eHITID0, eHITID1);
	}

}

Monster_A::~Monster_A()
{
}

void Monster_A::Update()
{
	Enemy::Update();
}