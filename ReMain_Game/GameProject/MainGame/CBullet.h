#pragma once

#include "../GEKO/GEKO.h"
//#include "../GEKO/Collider/ColliderManager.h"
//#include "../GEKO/Collider/LineSegment.h"

class CBullet  {
public:
	bool m_isActive;
//	LineSegment_vs_CapsuleCollider m_LineSegmentCollider;
	Vector3D m_pos;			//位置
	Vector3D m_Oldpos;		//移動前位置（線分判定用）
	int   m_cnt;			//寿命用カウンター
	Vector3D m_dir;			//方向
	float m_speed;		//速さ
	StaticMesh m_Model;	//モデル
	float m_Atk;		//攻撃力
	CBullet();
	~CBullet();
	bool UpDate();
	void Render();
//	void LineSegment_vs_CapsuleCallback(Result_Capsule& r);
};


class CBulletManager {
public:
	static CBulletManager *m_Obj;
	std::vector<CBullet*> m_Core;
	/*!
		@brief	コンストラクタ

	**/
	CBulletManager();
	~CBulletManager();
	/*!
		@brief　マネージャーの取得

	**/

	static CBulletManager* GetInstance() {
		if (!m_Obj) m_Obj = new CBulletManager();
		return m_Obj;

	}
	static void ClearInstance() {
		delete m_Obj;
		m_Obj = NULL;
	}
	/*!
		@brief	更新

	**/
	void UpDate();
	/*!
		@brief	描画

	**/
	void Render();

//	void CollisionMap(StaticMesh *m);
	/*!
		@brief	弾の生成
		@param	pos				[in] 座標
		@param	vec				[in] 移動量
		@retval	生成した弾オブジェクト

	**/
	CBullet *Add(const Vector3D pos, const Vector3D &dir,const float speed);

};