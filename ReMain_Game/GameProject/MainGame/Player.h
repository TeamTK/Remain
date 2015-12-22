#ifndef PlAYER_H
#define PlAYER_H

#include "../GEKO/GEKO.h"
#include "CCharacter.h"
#include "enums.h"

#include "../MainGame/Shotgun.h"
#include "../MainGame/Handgun.h"
#include "../GameSystem/SightSystem.h"
#include"../GameSystem/GUI/UI_SelectWeapon.h"

class Player : public CCharacter
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

private:
	Shotgun* m_pShotgun;
	Handgun* m_pHandgun;
	Image m_Reticle;		//���e�B�N���摜(��)
	SelectWeapon* m_SelectWeapon;

	Vector3D m_KeyDir;		//�L�[���͎�
	Vector3D m_PadDir;		//�R���g���[���[���͒l
	Vector3D m_CamDir;		//�J�����̕���
	Vector3D m_CameraPos;	//�J�����̈ʒu
	Vector3D m_LookPos;		//�J�����̒����_
	Vector3D m_Start;		//�����̓����蔻��p �n�_
	Vector3D m_End;			//�����̓����蔻��p �I�_
	Matrix m_Matrix;		//�V���b�g�K���̍s��
	EPlayerState m_State;	//�v���C���[�̏��	
	EWeapons m_SelectedWeapon;	//�I�����Ă���e
	Collider m_HitCamera;	//�J�����̓����蔻��
	Collider m_Bullet;		//�e�̓����蔻��
	PlayerSightInfo m_PlayerSightInfo;
	float m_Vertical;		//���������̃}�E�X���͗�
	float m_Horizontal;		//���������̃}�E�X���͗�
	float m_MoveSpeed;		//�ړ����x
	float m_CameraPosY;		//�J������Y���W
	float m_CamSpeed;		//�J�����̉�]���x
	int m_Phase;
	int m_AnimSpeed;		//�A�j���[�V�����X�s�[�h
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
