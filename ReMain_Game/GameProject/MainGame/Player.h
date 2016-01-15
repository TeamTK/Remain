#ifndef PlAYER_H
#define PlAYER_H

#include "../GEKO/GEKO.h"
#include "Character.h"
#include "enums.h"
#include "../MainGame/Weapon/Shotgun.h"
#include "../MainGame/Weapon/Handgun.h"
#include"../GameSystem/GUI/UI_SelectWeapon.h"
#include "../GameSystem/GUI/Number.h"

extern Vector3D *g_pPlayerPos;

class Player : public Character
{
public:
	Player(Vector3D pos);
	~Player();
	void Update();
	void Move();
	void Weapon();
	void Camera();
	void Animation();

private:
	void Idle();
	void Walk();
	void Run();
	void Crouch();
	void TakeWeapon();
	void PutBackWeapon();
	void SetupWeapon();
	void Reload();
	void Die();
	void Hit();

	void HitCamera(Result_Porygon &hitData);
	void HitAmmoBox(Result_Sphere& r);
	void HitEnemyAttack(Result_Capsule &hitData);

private:
	Number m_Num;
	UI_SelectWeapon m_SelectWeapon;
	Vector3D m_KeyDir;		//�L�[���͎�
	Vector3D m_PadDir;		//�R���g���[���[���͒l
	Vector3D m_CamDir;		//�J�����̕���
	Vector3D m_CameraPos;	//�J�����̈ʒu
	Vector3D m_LookPos;		//�J�����̒����_
	Vector3D m_Start;		//�����̓����蔻��p �n�_
	Vector3D m_End;			//�����̓����蔻��p �I�_
	Vector3D m_SightPos;	//��������̈ʒu
	Vector3D m_HitCameraPos; //�J�����̓����蔻��ʒu
	Matrix m_MatrixS;		//�V���b�g�K���̍s��
	Matrix m_MatrixH;		//�n���h�K���̍s��
	EPlayerState m_State;	//�v���C���[�̏��
	EPlayerState m_OldState;	//�v���C���[�̏��
	EPlayerAnim m_Anim;		//�v���C���[�̃A�j���[�V����
	EWeapons m_SelectedWeapon;	//�I�����Ă���e
	Collider m_HitCamera;	//�J�����̓����蔻��
	Collider m_Bullet;		//�e�̓����蔻��
	Collider m_HitAmmoBox;	//�e�򔠂̓����蔻��
	Collider m_HitEnemyAttack; //�G����̍U���̓����蔻��
	PlayerSightInfo m_PlayerSightInfo; //�v���C���[�̎��E���
	AuditoryObject m_AuditoryObject; //���o�Ώ�
	int m_Phase;
	int m_PlayAnim;			//����ɓn���p
	int m_Volume; //�v���C���[���o�����̉���
	float m_Vertical;		//���������̃}�E�X���͗�
	float m_Horizontal;		//���������̃}�E�X���͗�
	float m_MoveSpeed;		//�ړ����x
	float m_CameraPosY;		//�J������Y���W
	float m_CamSpeed;		//�J�����̉�]���x
	float m_AnimSpeed;		//�A�j���[�V�����X�s�[�h
	float m_PlayAnimTime;	//����ɓn���p
	float m_Radius;			//�e�򔠂Ƃ̓����蔻��̔��a
	bool m_ChangeTakeWeapon;	//�e������
	bool m_ChangePutBackWeapon;	//�e�����܂�
	bool m_ToggleCrouch;	//���Ⴊ�݃g�O��
	bool m_isTakeWeapon;	//�e�������Ă��邩
	bool m_SetupWeapon;	//�e���\���Ă��邩
	bool m_isCrouch;	//���Ⴊ��ł��邩
	bool m_isRun;		//�����Ă��邩
	bool m_isMove;		//�ړ�����
	bool m_isAttack;	//�U������
	bool m_isReload;	//�����[�h����
	bool m_isHit;		//�G�̍U��������������
};

#endif
