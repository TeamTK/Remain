#ifndef PlAYER_H
#define PlAYER_H

#include "../GEKO/GEKO.h"
#include "CCharacter.h"

//�v���C���[�̃A�j���[�V�������
enum EPlayerAnimation
{
	eCrouch = 0,		//���Ⴊ�݈ڍs
	eCrouchIdle,		//���Ⴊ�ݑҋ@
	eCrouchWalk,		//���Ⴊ�ݕ���
	eDie,				//����
	eHit,				//��U��
	eIdle,				//�ҋ@���
	eIdleTakeGun,		//�V���b�g�K�������ҋ@���
	eIdleTakeHandgun,	//�n���h�K�������ҋ@���
	eRun,				//����
	eRunTakeGun,		//�V���b�g�K���������đ���
	eRunTakeHandgun,	//�n���h�K���������đ���
	eSetupGun,			//�V���b�g�K������������Ԃ���\����
	eSetupHandgun,		//�n���h�K������������Ԃ���\����
	eTakeGun,			//�V���b�g�K������Ɏ���
	eTakeHandgun,		//�n���h�K������Ɏ���
	eWalk,				//����
	eWalkTakeGun,		//�V���b�g�K���������ĕ���
	eWalkTakeHandgun	//�n���h�K���������ĕ���
};

//�v���C���[�̏��
enum EPlayerState
{
	eWait = 0,
	eMove
};

enum EWeapons
{
	eShotgun = 0,
	eHandgun
};

class Player : public CCharacter
{
public:
	Player();
	~Player();
	void Update();
//	void Render();
	void Move();
	void Attack();
	void Camera();
	void Animation();
	Matrix GetBomeMat(int bornIndex);
	int GetAnim();
	float GetPlayTime();

private:
//	DynamicMesh m_Player;
	Vector3D m_Dir;
	EPlayerAnimation m_Anim;
	EPlayerState m_State;
	EWeapons m_Weapons;
	float m_Vertical;
	float m_Horizontal;
	float m_MoveSpeed;
	float m_CameraPosY;
	bool m_isMove;
	bool m_isCrouch;
	bool isTakeWeapons;
	bool m_isAttack;
};

class Shotgun
{
public:
	Shotgun(Player* p);
	~Shotgun();
	void Update();
	void Render();

private:
	Player* m_pPlayer;
	StaticMesh m_Shotgun;
	Matrix m_BoneMat;


};

class Handgun
{
public:
	Handgun(Player* p);
	~Handgun();
	void Update();
	void Render();

private:
	Player* m_pPlayer;
	StaticMesh m_Handgun;
	Matrix m_BoneMat;

};

#endif
