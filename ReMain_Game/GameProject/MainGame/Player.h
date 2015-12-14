#ifndef PlAYER_H
#define PlAYER_H

#include "../GEKO/GEKO.h"
#include "CCharacter.h"

//プレイヤーのアニメーション
enum EPlayerAnimation
{
	eAnim_Crouch,			//しゃがみ移行
	eAnim_CrouchIdle,		//しゃがみ待機
	eAnim_CrouchWalk,		//しゃがみ歩き
	eAnim_Die,				//やられ
	eAnim_Hit,				//被攻撃
	eAnim_Idle,				//待機状態
	eAnim_IdleTakeGun,		//ショットガン持ち待機状態
	eAnim_IdleTakeHandgun,	//ハンドガン持ち待機状態
	eAnim_Run,				//走り
	eAnim_RunTakeGun,		//ショットガンを持って走る
	eAnim_RunTakeHandgun,	//ハンドガンを持って走る
	eAnim_SetupGun,			//ショットガンを持った状態から構える
	eAnim_SetupHandgun,		//ハンドガンを持った状態から構える
	eAnim_TakeGun,			//ショットガンを手に持つ
	eAnim_TakeHandgun,		//ハンドガンを手に持つ
	eAnim_Walk,				//歩く
	eAnim_WalkTakeGun,		//ショットガンを持って歩く
	eAnim_WalkTakeHandgun	//ハンドガンを持って歩く
};

//プレイヤーの状態
enum EPlayerState
{
	eState_Idle,
	eState_Walk,
	eState_Run,
	eState_Crouch,
	eState_TakeWeapon,
	eState_SetupWeapon,
	eState_Hit,
	eState_Die,
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

	void Idle();
	void Walk();
	void Run();
	void Crouch();
	void TakeWeapon();
	void SetupWeapon();
	void Die();
	void Hit();

private:
	void HitCamera(Result_Porygon &hitData);
	Vector3D Lerp(Vector3D start, Vector3D finish, float percentage);

private:
	Vector3D m_Dir;
	Vector3D m_CamDir;
	Vector3D m_CameraPos;
	Vector3D m_LookPos;
	Matrix m_GunMatrix;
	EPlayerAnimation m_Anim;
	EPlayerState m_State;
	EWeapons m_Weapons;
	Collider m_HitCamera;
	float m_Vertical;
	float m_Horizontal;
	float m_MoveSpeed;
	float m_CameraPosY;
	float m_CameraT;
	bool m_ChangeTakeWeapon;	//銃を持つ
	bool m_isTakeWeapon;	//銃を持っているか
	bool m_isCrouch;	//しゃがんでいるか
	bool m_isRun;	//走っているか
	bool m_isMove;		//移動中か
	bool m_isAttack;	//攻撃中か
};
#endif
