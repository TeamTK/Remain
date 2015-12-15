#ifndef WEAPON_BASE_H
#define WEAPON_BASE_H

#include "../GEKO/GEKO.h"
#include "enums.h"

class WeaponBase
{
public:
	WeaponBase();
	virtual ~WeaponBase();
	void Update();
	void Render();
	void Shot();
	void LineSegment_vs_CapsuleCallback();
	void SetPlayerData(int anim, int frame, Vector3D start, Vector3D end);
	void SetPlayerBomeMtx(Matrix* m);
	Matrix GetMatrix();		//銃の行列取得

protected:
	int m_Ammo;		//弾数
	StaticMesh m_Model;
	Collider m_Collider;
	Matrix m_Matrix;	//銃の行列

	int m_PlayerAnim;		//プレイヤーの再生中アニメーション
	int m_PlayerAnimFrame;	//プレイヤーのアニメーションフレーム
	Matrix* m_BoneMtx;		//プレイヤーのボーン行列
	Vector3D m_Start;	//線分の当たり判定用 始点
	Vector3D m_End;		//線分の当たり判定用 終点
};

#endif