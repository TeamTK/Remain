#ifndef WEAPON_BASE_H
#define WEAPON_BASE_H

#include "../../GEKO/GEKO.h"
#include "../enums.h"

#define AMMO_LOADED_SHOTGUN 6	//最大装弾数ショットガン
#define AMMO_LOADED_HANDGUN 6	//最大装弾数ハンドガン

class WeaponBase : public Task
{
public:
	WeaponBase();
	WeaponBase(int* anim, float* frame, Matrix* m, const char *taskName, unsigned int priority);
	virtual ~WeaponBase();
	void Update();
	void Render();
	void Shot();
	int GetAmmo();	//合計残弾数を取得
	int GetLoadedAmmo();
	bool CanShot();	//弾を撃てるか
	void ReduceBullet();	//弾を減らす
	void AddAmmo(int addnum);	//弾を増やす
	void Reload();		//弾をリロード

protected:
	int m_Ammo;		//手持ち弾数
	int m_LoadedAmmo;	//銃に入っている弾数
	int* m_PlayerAnim;		//プレイヤーの再生中アニメーション
	float* m_PlayerAnimFrame;	//プレイヤーのアニメーションフレーム
	StaticMesh m_Model;
	Matrix* m_BoneMtx;		//プレイヤーのボーン行列
	RenderTask m_RenderTask;
};

#endif