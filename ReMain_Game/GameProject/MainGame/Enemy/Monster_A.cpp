#include "Monster_A.h"

#define MONSTER_A_ATTACK_ENDTIME 29
#define MONSTER_A_HITDAMAGE_ENDTIME 29
#define MONSTER_A_DIE_ENDTIME 29

Monster_A::Monster_A(EnemyState &enemyState) :
	Enemy("Monster_A", enemyState)
{
	//�e���ʂ̃J�v�Z���̏��
	m_BoneCapsule.emplace_back(0.4f, 25, 4);	//��
	m_BoneCapsule.emplace_back(0.6f, 2, 4);   //����
	m_BoneCapsule.emplace_back(0.5f, 6, 8);   //���r
	m_BoneCapsule.emplace_back(0.5f, 16, 18); //�E�r
	m_BoneCapsule.emplace_back(0.5f, 26, 28); //����
	m_BoneCapsule.emplace_back(0.5f, 30, 32); //�E��

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
	m_FuncTask.Regist("Attack", REGIST_FUNC_TASK(Monster_A::Attack));
	m_FuncTask.Regist("Idle", REGIST_FUNC_TASK(Monster_A::Idle));
	m_FuncTask.Regist("Chase", REGIST_FUNC_TASK(Monster_A::Chase));
	m_FuncTask.Regist("HitDamage", REGIST_FUNC_TASK(Monster_A::HitDamage));
	m_FuncTask.Regist("Die", REGIST_FUNC_TASK(Monster_A::Die));

	//�X�V��~
	m_FuncTask.Sleep("Attack");
	m_FuncTask.Sleep("Chase");
	m_FuncTask.Sleep("HitDamage");
	m_FuncTask.Sleep("Die");
}

Monster_A::~Monster_A()
{
}

void Monster_A::Attack()
{
	Enemy::Attack(eAnimationAttack, MONSTER_A_ATTACK_ENDTIME);
}

void Monster_A::Idle()
{
	Enemy::Idle(eAnimationIdle);
}

void Monster_A::Chase()
{
	Enemy::Chase(eAnimationTrot);
}

void Monster_A::HitDamage()
{
	Enemy::HitDamage(eAnimationHitDamage, MONSTER_A_HITDAMAGE_ENDTIME);
}

void Monster_A::Die()
{
	Enemy::Die(eAnimationDie, MONSTER_A_DIE_ENDTIME);
}

void Monster_A::Update()
{
	Enemy::Update();
}