#include "Monster_A.h"

#define MONSTER_A_ATTACK_ENDTIME 29
#define MONSTER_A_HITDAMAGE_ENDTIME 29
#define MONSTER_A_DIE_ENDTIME 29

#define MONSTER_A_ATTACK_SPEED 20.0f

Monster_A::Monster_A(EnemyState &enemyState) :
	Enemy("Monster_A", enemyState)
{
	//�e���ʂ̃J�v�Z���̏��
	m_BoneCapsule.emplace_back(0.4f, 25, 25, "Head");		  //��
	m_BoneCapsule.emplace_back(0.6f, 2, 5, "Body");         //����
	m_BoneCapsule.emplace_back(0.5f, 6, 8, "Left arm");     //���r
	m_BoneCapsule.emplace_back(0.5f, 16, 18, "Right arm");  //�E�r
	m_BoneCapsule.emplace_back(0.5f, 26, 28, "Left leg");   //����
	m_BoneCapsule.emplace_back(0.5f, 30, 32, "Right leg");  //�E��

	//�_���[�W�{�����ʂ���
	m_DamageMagnification.push_back(2.0f); //��
	m_DamageMagnification.push_back(2.0f); //����
	m_DamageMagnification.push_back(0.5f); //���r
	m_DamageMagnification.push_back(0.5f); //�E�r
	m_DamageMagnification.push_back(0.5f); //����
	m_DamageMagnification.push_back(0.5f); //�E��

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
		m_pCollider[i].SetName(m_BoneCapsule[i].name);

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

	m_FuncTask.AllStop();
	m_FuncTask.Start("Idle");
}

Monster_A::~Monster_A()
{
}

void Monster_A::Attack()
{
	if(m_Model.GetPlayTime() == 10) m_pHitAttack[3].Awake(); //�E�r�̓����蔻��N��
	if (m_Model.GetPlayTime() == 20) m_pHitAttack[3].Sleep(); //�E�r�̓����蔻��I��
	m_AnimSpeed = MONSTER_A_ATTACK_SPEED;

	m_AnimType = eAnimationAttack;
	m_AnimEndTime = MONSTER_A_ATTACK_ENDTIME;
	Enemy::Attack();
}

void Monster_A::Idle()
{
	m_AnimType = eAnimationIdle;
	Enemy::Idle();
}

void Monster_A::Chase()
{
	m_AnimType = eAnimationTrot;
	Enemy::Chase();
}

void Monster_A::HitDamage()
{
	m_AnimType = eAnimationHitDamage;
	m_AnimEndTime = MONSTER_A_HITDAMAGE_ENDTIME;
	Enemy::HitDamage();
}

void Monster_A::Die()
{
	m_AnimType = eAnimationDie;
	m_AnimEndTime = MONSTER_A_DIE_ENDTIME;
	m_AnimSpeed = ENEMY_NORMAL_SPEED;
	Enemy::Die();
}

void Monster_A::Update()
{
	Enemy::Update();
}