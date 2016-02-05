#ifndef _PLAYER_INFO_H_
#define _PLAYER_INFO_H_

#include "../Character.h"

#define DeadValue 0.1f	//パッド入力のデッド値

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
	eAnim_RecoilGun,		//ショットガン反動
	eAnim_RecoilHandgun,	//ハンドガン反動
	eAnim_ReloadGun,		//ショットガンリロード
	eAnim_ReloadHandgun,	//ハンドガンリロード
	eAnim_Run,				//走り
	eAnim_RunTakeGun,		//ショットガンを持って走る
	eAnim_RunTakeHandgun,	//ハンドガンを持って走る
	eAnim_SetupGun,			//ショットガンを持った状態から構える
	eAnim_SetupHandgun,		//ハンドガンを持った状態から構える
	eAnim_StealthAttack,	//ステルスアタック
	eAnim_TakeGun,			//ショットガンを手に持つ
	eAnim_TakeHandgun,		//ハンドガンを手に持つ
	eAnim_Walk,				//歩く
	eAnim_WalkTakeGun,		//ショットガンを持って歩く
	eAnim_WalkTakeHandgun,	//ハンドガンを持って歩く
	eAnim_WalkReloadGun,	//リロード中歩き
	eAnim_WalkReloadHandgun	//リロード中歩き
};

//プレイヤーの状態
enum EPlayerState
{
	eState_Idle,
	eState_Walk,
	eState_Run,
	eState_Crouch,
	eState_StandUp,
	eState_TakeWeapon,
	eState_PutBackWeapon,
	eState_SetupWeapon,
	eState_StealthAttack,
	eState_Recoil,
	eState_Reload,
	eState_Hit,
	eState_Die,
};

struct CameraInfo
{
	bool *pIsStop;
	bool *pSetupWeapon;	//銃を構えているか
	DynamicMesh *pModel;
	Vector3D *pPos;
	Vector3D *pRot;
	Vector3D *pLookPos;	//カメラの注視点
	EPlayerState *pState;
};

#endif