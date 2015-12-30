#ifndef PlAYER_H
#define PlAYER_H

#include "../GEKO/GEKO.h"
#include "Character.h"
#include "enums.h"

#include "../MainGame/Shotgun.h"
#include "../MainGame/Handgun.h"
#include "../GameSystem/SightSystem.h"
#include"../GameSystem/GUI/UI_SelectWeapon.h"

extern Vector3D *g_pPlayerPos;

class Player : public Character
{
public:
	Player();
	~Player();
	void Update();
	void Move();
	void Attack();
	void Camera();
	void Animation();
	Matrix GetBomeMat(int bornIndex);
	int GetAnim();
	float GetPlayTime();

private:
	void Idle();
	void Walk();
	void Run();
	void Crouch();
	void TakeWeapon();
	void PutBackWeapon();
	void SetupWeapon();
	void Die();
	void Hit();

	void HitCamera(Result_Porygon &hitData);
	void HitAmmoBox(Result_Sphere& r);
	void HitEnemyAttack(Result_Capsule &hitData);

private:
	Shotgun m_pShotgun;
	Handgun m_pHandgun;
	UI_SelectWeapon m_SelectWeapon;

	Vector3D m_KeyDir;		//�L�[���͎�
	Vector3D m_PadDir;		//�R���g���[���[���͒l
	Vector3D m_CamDir;		//�J�����̕���
	Vector3D m_CameraPos;	//�J�����̈ʒu
	Vector3D m_LookPos;		//�J�����̒����_
	Vector3D m_Start;		//�����̓����蔻��p �n�_
	Vector3D m_End;			//�����̓����蔻��p �I�_
	Vector3D m_SightPos;	//��������̈ʒu
	Matrix m_Matrix;		//�V���b�g�K���̍s��
	EPlayerState m_State;	//�v���C���[�̏��	
	EWeapons m_SelectedWeapon;	//�I�����Ă���e
	Collider m_HitCamera;	//�J�����̓����蔻��
	Collider m_Bullet;		//�e�̓����蔻��
	Collider m_HitAmmoBox;	//�e�򔠂̓����蔻��
	Collider m_HitEnemyAttack; //�G����̍U���̓����蔻��
	PlayerSightInfo m_PlayerSightInfo; //�v���C���[�̎��E���
	float m_Vertical;		//���������̃}�E�X���͗�
	float m_Horizontal;		//���������̃}�E�X���͗�
	float m_MoveSpeed;		//�ړ����x
	float m_CameraPosY;		//�J������Y���W
	float m_CamSpeed;		//�J�����̉�]���x
	float m_AnimSpeed;		//�A�j���[�V�����X�s�[�h
	float m_Radius;
	int m_Phase;
	bool m_ChangeTakeWeapon;	//�e������
	bool m_ToggleCrouch;
	bool m_isTakeWeapon;	//�e�������Ă��邩
	bool m_SetupWeapon;	//�e���\���Ă��邩
	bool m_isCrouch;	//���Ⴊ��ł��邩
	bool m_isRun;		//�����Ă��邩
	bool m_isMove;		//�ړ�����
	bool m_isAttack;	//�U������
};
#endif
