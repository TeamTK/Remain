#ifndef PlAYER_H
#define PlAYER_H

#include "../../MainGame/Weapon/Shotgun.h"
#include "../../MainGame/Weapon/Handgun.h"
#include "../../MainGame/Weapon/Knife.h"
#include "PlayerInfo.h"
#include "PlayerData.h"

extern Vector3D *g_pPlayerPos;

class Player : public Character
{
public:
	Player(PData* data, Vector3D pos, float rotY, float horizontal, float vertical);
	~Player();
	void Update();
	void Move();
	void Attack();
	void Animation();

private:
	void Idle();
	void Walk();
	void Run();
	void Crouch();
	void StandUp();
	void TakeWeapon();
	void PutBackWeapon();
	void SetupWeapon();
	void Recoil();
	void Reload();
	void StealthAttack();
	void Die();
	void Hit();

	void HitAmmoBox(Result_Sphere& r);
	void HitEnemyAttack(Result_Capsule &hitData);
	void StageChange(Result_Sphere &data);
	void HitRecoveryItem(Result_Sphere &data);

private:
	Vector3D m_KeyDir;		//�L�[���͎�
	Vector3D m_LookPos;		//�J�����̒����_
	Vector3D m_SightPos;	//��������̈ʒu
	Matrix m_MatrixS;		//�V���b�g�K���p�v���C���[�̍s��
	Matrix m_MatrixH;		//�n���h�K���p�v���C���[�̍s��
	Matrix m_MatrixK;		//�i�C�t�p�v���C���[�̍s��
	EPlayerState m_State;	//�v���C���[�̏��
	EPlayerAnim m_Anim;		//�v���C���[�̃A�j���[�V����
	EWeapons m_SelectedWeapon; //�I�����Ă���e
	PlayerSightInfo m_PlayerSightInfo; //�v���C���[�̎��E���
	AuditoryObject m_AuditoryObject; //���o�Ώ�
	Timer m_Timer;					//�_���[�W��H�������̖��G����
	Timer m_NoActionTime;			//���[�h���̖����쎞��
	TracerouteTarget m_Target;		//�o�H�T���Ώ�

	Collider m_Bullet;		//�e�̓����蔻��
	Collider m_HitAmmoBox;	//�e�򔠂̓����蔻��
	Collider m_HitEnemyAttack; //�G����̍U���̓����蔻��
	Collider m_StageChange; //�X�e�[�W�ړ��p
	Collider m_HitRecoveryItem; //�񕜃A�C�e���̓����蔻��

	Sound m_HandGunFire;			//�n���h�K���̔��C��
	Sound m_ShotGunFire;			//�V���b�g�K���̔��C��
	Sound m_GunEmpty;			    //�e���e�؂�ɂȂ������̉�
	Sound m_GunADS;					//�e���\�������̉�
	Sound m_HitEnemyAttackSound;	//�G����U����H�������

	Image m_DamegeBlood;

	CameraInfo m_CameraInfo;

	int m_Phase;
	int m_PlayAnim;			//����ɓn���p
	int m_Volume;			//�v���C���[���o�����̉���
	int m_RecoveryItemNum;	//�񕜖�̌g�ѐ�

	float m_MoveSpeed;		//�ړ����x
	float m_AnimSpeed;		//�A�j���[�V�����X�s�[�h
	float m_PlayAnimTime;	//����ɓn���p
	float m_Radius;			//�e�򔠂Ƃ̓����蔻��̔��a
	float m_OneFlameTime;	//1�t���[���̎���

	bool m_ChangeTakeWeapon;		//�e������
	bool m_ChangePutBackWeapon;	//�e�����܂�
	bool m_ToggleCrouch;			//���Ⴊ�݃g�O��
	bool m_isCrouch;				//���Ⴊ��ł��邩
	bool m_isShiftCrouch;			//���Ⴊ��ł��邩
	bool m_isTakeWeapon;			//�e�������Ă��邩
	bool m_SetupWeapon;			//�e���\���Ă��邩
	bool m_isRun;					//�����Ă��邩
	bool m_isMove;					//�ړ�����
	bool m_isAttack;				//�U������
	bool m_isReload;				//�����[�h����
	bool m_isHit;					//�G�̍U��������������
	bool m_isShot;					//���C������
	bool m_isSteAttack;			//�X�e���X�A�^�b�N
	bool m_isDead;					//���S���f
	bool m_IsStop;					//�v���C���[�Ɋ֌W����^�X�N��~
};

#endif
