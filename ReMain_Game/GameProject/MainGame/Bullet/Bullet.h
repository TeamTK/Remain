#ifndef _BULLET_H_
#define _BULLET_H_

//#include "../../GEKO/GEKO.h"
#include "../../GEKO/Collider/Collider.h"
#include "../../GEKO/Task/Task.h"

class Bullet : public Task
{
public:
	Bullet(const Vector3D &start, const Vector3D &dir, float speed, float time, float radius);
	~Bullet();
	void Update();
	void HitCharacter(Result_Capsule& r);
	void HitMap(Result_Porygon& r);

private:
	Collider m_ColliderMap; //マップとの当たり判定
	Collider m_Collider; //キャラクタとの当たり判定
	Vector3D m_Pos;		//位置
	Vector3D m_Oldpos;	//移動前位置（線分判定用）
	Vector3D m_Dir;		//方向
	float m_Cnt;		//寿命用カウンター
	float m_Time;		//寿命時間
	float m_Speed;		//速さ
	float m_Radius;
};

#endif