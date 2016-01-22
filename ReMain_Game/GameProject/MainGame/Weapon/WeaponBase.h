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
	WeaponBase(int* anim, float* frame, bool* take, Matrix* m, const char *taskName, unsigned int priority);
	virtual ~WeaponBase();
	void Update();
	void Render();
	void Shot();
	int GetAmmo();			//携帯弾数を取得
	int GetLoadedAmmo();	//武器にロードしている弾数を取得
	int GetAllAmmo();		//すべての弾薬数を取得
	bool CanShot();			//弾を撃てるか
	void ReduceBullet();	//弾を減らす
	void AddAmmo(int addnum);	//弾を増やす
	void Reload();			//弾をリロード
	EWeapons GetSelect();

protected:
	int m_Ammo;				//手持ち弾数
	int m_LoadedAmmo;		//銃に入っている弾数
	int* m_PlayerAnim;		//プレイヤーの再生中アニメーション
	float* m_PlayerAnimFrame;	//プレイヤーのアニメーションフレーム
	bool* m_TakeWeapon;		//プレイヤーが武器を持っているか
	StaticMesh m_Model;
	Matrix* m_BoneMtx;		//プレイヤーのボーン行列
	RenderTask m_RenderTask;
	EWeapons* m_Weapon;
};

#endif