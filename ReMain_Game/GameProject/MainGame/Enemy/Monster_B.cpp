#include "Monster_B.h"

#define MONSTER_B_ATTACK_ENDTIME 29
#define MONSTER_B_HITDAMAGE_ENDTIME 29
#define MONSTER_B_DIE_ENDTIME 29

Monster_B::Monster_B(Vector3D pos, Vector3D rot) :
	Enemy(pos, rot, "Monster_B", 2)
{
	//�e���ʂ̃J�v�Z���̏��
	m_BoneCapsule.emplace_back(0.4f, 13, 14);	//��
	m_BoneCapsule.emplace_back(0.6f, 2, 13);   //����
	m_BoneCapsule.emplace_back(0.5f, 6, 8);   //���r
	m_BoneCapsule.emplace_back(0.5f, 10, 12); //�E�r
	m_BoneCapsule.emplace_back(0.5f, 15, 18); //����
	m_BoneCapsule.emplace_back(0.5f, 19, 22); //�E��

	//���i�e�j�Ƃ̔���p
	m_pCollider = new Collider[m_BoneCapsule.size()];
	m_pCapsule = new CapsuleInfo[m_BoneCapsule.size()];

	//�U���̔���
	m_pHitAttack = new Collider[m_BoneCapsule.size()];

	unsigned int bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		m_pCollider[i].Regist_C_vs_S(&m_pCapsule[i].segment.start, &m_pCapsule[i].segment.end,
			&m_pCapsule[i].radius, REGIST_FUNC(Enemy::HitBullet));
		m_pCollider[i].SetID(eHITID0, eHITID1);

		m_pHitAttack[i].Regist_C_vs_C(&m_pCapsule[i].segment.start, &m_pCapsule[i].segment.end,
			&m_pCapsule[i].radius, REGIST_FUNC(Enemy::HitAttack));
		m_pHitAttack[i].SetID(eHITID1, eHITID0);
		m_pHitAttack[i].Sleep();
	}

	//�֐���o�^
	m_FuncTask.Regist("Attack", REGIST_FUNC_TASK(Monster_B::Attack));
	m_FuncTask.Regist("Idle", REGIST_FUNC_TASK(Monster_B::Idle));
	m_FuncTask.Regist("Chase", REGIST_FUNC_TASK(Monster_B::Chase));
	m_FuncTask.Regist("HitDamage", REGIST_FUNC_TASK(Monster_B::HitDamage));
	m_FuncTask.Regist("Die", REGIST_FUNC_TASK(Monster_B::Die));

	//�X�V��~
	m_FuncTask.Sleep("Attack");
	m_FuncTask.Sleep("Chase");
	m_FuncTask.Sleep("HitDamage");
	m_FuncTask.Sleep("Die");
}

Monster_B::~Monster_B()
{
}

void Monster_B::Attack()
{
	Enemy::Attack(eAnimationAttack, MONSTER_B_ATTACK_ENDTIME);
}

void Monster_B::Idle()
{
	Enemy::Idle(eAnimationIdle);
}

void Monster_B::Chase()
{
	Enemy::Chase(eAnimationTrot);
}

void Monster_B::HitDamage()
{
	Enemy::HitDamage(eAnimationHitDamage, MONSTER_B_HITDAMAGE_ENDTIME);
}

void Monster_B::Die()
{
	Enemy::Die(eAnimationDie, MONSTER_B_DIE_ENDTIME);
}

void Monster_B::Update()
{
	Enemy::Update();
}