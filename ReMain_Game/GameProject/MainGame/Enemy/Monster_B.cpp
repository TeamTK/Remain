#include "Monster_B.h"

#define MONSTER_B_ATTACK_ENDTIME 29
#define MONSTER_B_HITDAMAGE_ENDTIME 29
#define MONSTER_B_DIE_ENDTIME 29

Monster_B::Monster_B(EnemyState &enemyState) :
	Enemy("Monster_B", enemyState)
{
	//�e���ʂ̃J�v�Z���̏��
	m_BoneCapsule.emplace_back(0.4f, 15, 16, "Head");		 //��
	m_BoneCapsule.emplace_back(0.6f, 2, 5, "Body");	     //����
	m_BoneCapsule.emplace_back(0.5f, 6, 8, "Left arm");    //���r
	m_BoneCapsule.emplace_back(0.5f, 10, 13, "Right arm"); //�E�r
	m_BoneCapsule.emplace_back(0.5f, 17, 19, "Left leg");  //����
	m_BoneCapsule.emplace_back(0.5f, 21, 23, "Rgiht leg"); //�E��

	//�_���[�W�{�����ʂ���
	m_DamageMagnification.push_back(4.0f); //��
	m_DamageMagnification.push_back(2.5f); //����
	m_DamageMagnification.push_back(1.0f); //���r
	m_DamageMagnification.push_back(1.0f); //�E�r
	m_DamageMagnification.push_back(1.0f); //����
	m_DamageMagnification.push_back(1.0f); //�E��

	//���i�e�j�Ƃ̔���p
	m_pHitAttackBody = new Collider[m_BoneCapsule.size()];
	m_pCapsule = new CapsuleInfo[m_BoneCapsule.size()];

	//�U���̓����蔻��
	m_pHitAttack = new Collider[m_BoneCapsule.size()];

	unsigned int bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		//�v���C���[����̍U���̓����蔻��
		m_pHitAttackBody[i].Regist_C_vs_S(&m_pCapsule[i].segment.start, &m_pCapsule[i].segment.end,
			&m_pCapsule[i].radius, REGIST_FUNC(Enemy::HitBullet));
		m_pHitAttackBody[i].SetID(eHITID0, eHITID1);
		m_pHitAttackBody[i].SetName(m_BoneCapsule[i].name);

		//�G����̍U���̓����蔻��
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

	//�o�H�T�����f
	m_FuncTask.AllStop();
	if (enemyState.isSearch)
	{
		m_FuncTask.Start("Chase");
	}
	else
	{
		m_FuncTask.Start("Idle");
	}

	m_JudgementAnim = 13;

	m_DamegeBlood.SetAsset("Monster_B_2");
}

Monster_B::~Monster_B()
{
}

void Monster_B::Attack()
{
	float animNum = m_Model.GetPlayTime(13);

	//�������ߍU�������蔻��J�n
	if (animNum >= 10)
	{
		m_pHitAttack[2].Awake(); //���r�̓����蔻��N��
		m_pHitAttack[3].Awake(); //�E�r�̓����蔻��N��
	}

	//�������ߍU�������蔻��I��
	if (animNum >= 20)
	{
		m_pHitAttack[2].Sleep(); //���r�̓����蔻��N��
		m_pHitAttack[3].Sleep(); //�E�r�̓����蔻��N��
	}

	m_AnimType = eAnimationAttack;
	m_AnimEndTime = MONSTER_B_ATTACK_ENDTIME;
	Enemy::Attack();
}

void Monster_B::Idle()
{
	m_AnimType = eAnimationIdle;
	Enemy::Idle();
}

void Monster_B::Chase()
{
	m_AnimType = eAnimationTrot;
	Enemy::Chase();
}

void Monster_B::HitDamage()
{
	m_AnimType = eAnimationHitDamage;
	m_AnimEndTime = MONSTER_B_HITDAMAGE_ENDTIME;

	Enemy::HitDamage();
}

void Monster_B::Die()
{
	m_AnimType = eAnimationDie;
	m_AnimEndTime = MONSTER_B_DIE_ENDTIME;
	m_AnimSpeed = ENEMY_NORMAL_SPEED;
	Enemy::Die();
}

void Monster_B::Update()
{
	Enemy::Update();
}