#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "../Character.h"

#define ENEMY_NORMAL_SPEED 30.0f

//�G�̃X�e�[�^�X
struct EnemyState
{
	bool isSearch;
	int flinch;
	float hp;
	float walkSpeed;
	float runSpeed;
	float mapHitRadius;
	float bodyRadius;
	float sightAngle;
	float sightDistance;
	const char* topographyName;
	Vector3D posSpawn;
	Vector3D rotation;
};

//�{�[���̃J�v�Z�����
struct BoneCalpule
{
	float radius;
	int start;
	int end;
	const char *name;
	BoneCalpule(float r, int s, int e, const char* name) :
		radius(r),
		start(s),
		end(e),
		name(name) {}
};

//���̓G
class Enemy : public Character
{
public:
	Enemy(const char* name, EnemyState &enemyState);
	virtual ~Enemy();
	virtual void Update();
	void HitBullet(Result_Sphere& r);
	void HitAttack(Result_Capsule &hitData);

protected:
	void Attack();
	void Idle();
	void Chase();
	void HitDamage();
	void Die();
	void Wanderings();

private:
	void HitSight(const Vector3D *pPos);
	void Auditory(int volume);

protected:
	unsigned int m_AnimType; //�A�j���[�V�����̎��
	int m_FlinchNum; //�ϋv�l
	float m_FlinchCnt; //�ϋv�~�ϒl
	float m_WalkSpeed; //�������x
	float m_RunSpeed; //���鑬�x
	float m_AnimSpeed; //�A�j���[�V�����̑��x
	float m_AnimEndTime; //�A�j���[�V�����̏I��莞��

	//�G�̎��E
	EnemySight m_Sight; //���E�V�X�e��
	SightData m_SightData; //���E�̃f�[�^
	Vector3D m_SightPos; //���E�̈ʒu
	Vector3D m_SightVec; //���E�̕���

	//���o�T�m
	enum
	{
		eOblivious = 1, eCaution = 2, eDiscovery = 3
	};
	float m_AuditoryRange; //�T�m�͈�
	AuditorySense m_AuditorySense; //�T�m�N���X

	//�ǐ՗p
	Vector3D m_Distance; //�G�ƃv���C���[�Ƃ̋���
	const Vector3D *m_pPlayerPos; //�v���C���[�̈ʒu�̃|�C���^

	//�v���C���[����̍U�����ꂽ���̓����蔻��
	Collider *m_pHitAttackBody;
	CapsuleInfo *m_pCapsule;
	std::vector<BoneCalpule> m_BoneCapsule; //�{�[�����i�J�v�Z���j
	std::vector<float> m_DamageMagnification; //�_���[�W�{��

	Collider *m_pHitAttack; //�v���C���[�ւ̍U���̓����蔻��
	FunctionTask m_FuncTask; //AI�Ǘ�

	TracerouteSearch m_Search; //�o�H�T��

	int m_SearchCnt; //�o�H�T���Ԋu�J�E���g
};

#endif