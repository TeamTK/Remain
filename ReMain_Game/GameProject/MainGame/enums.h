#ifndef ENUMS_H
#define ENUMS_H

//プレイヤーのアニメーション
enum EPlayerAnim
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
	eState_PutBackWeapon,
	eState_SetupWeapon,
	eState_Reload,
	eState_Hit,
	eState_Die,
};

//武器
enum EWeapons
{
	eHand,
	eShotgun,
	eHandgun,
	eNone,
	WeaponAll
};

#endif