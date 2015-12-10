#ifndef PlAYER_H
#define PlAYER_H

#include "../GEKO/GEKO.h"
#include "CCharacter.h"

//プレイヤーのアニメーション状態
enum EPlayerAnimation
{
	eCrouch = 0,		//しゃがみ移行
	eCrouchIdle,		//しゃがみ待機
	eCrouchWalk,		//しゃがみ歩き
	eDie,				//やられ
	eHit,				//被攻撃
	eIdle,				//待機状態
	eIdleTakeGun,		//ショットガン持ち待機状態
	eIdleTakeHandgun,	//ハンドガン持ち待機状態
	eRun,				//走り
	eRunTakeGun,		//ショットガンを持って走る
	eRunTakeHandgun,	//ハンドガンを持って走る
	eSetupGun,			//ショットガンを持った状態から構える
	eSetupHandgun,		//ハンドガンを持った状態から構える
	eTakeGun,			//ショットガンを手に持つ
	eTakeHandgun,		//ハンドガンを手に持つ
	eWalk,				//歩く
	eWalkTakeGun,		//ショットガンを持って歩く
	eWalkTakeHandgun	//ハンドガンを持って歩く
};

//プレイヤーの状態
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
	void Move();
	void Attack();
	void Camera();
	void Animation();
	Matrix GetBomeMat(int bornIndex);
	int GetAnim();
	float GetPlayTime();
	void SetGunMtx(Matrix m);

private:
	Vector3D m_Dir;
	Vector3D m_CamDir;
	Matrix m_GunMatrix;
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
	Matrix GetMatrix();

private:
	Player* m_pPlayer;
	StaticMesh m_Shotgun;
	Matrix m_BoneMat;
	Matrix m_Matrix;

};

class Handgun
{
public:
	Handgun(Player* p);
	~Handgun();
	void Update();
	void Render();
	Matrix GetMatrix();

private:
	Player* m_pPlayer;
	StaticMesh m_Handgun;
	Matrix m_BoneMat;
	Matrix m_Matrix;

};

#endif
