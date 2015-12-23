#ifndef _BULLET_H_
#define _BULLET_H_

#include "../GEKO/GEKO.h"
#include "../GEKO/Collider/Collider.h"

class Bullet : public Task
{
public:
	Bullet(const Vector3D &start, const Vector3D &dir, float speed, float time);
	~Bullet();
	void Update();
	void Render();
	void HitCharacter(Result_Capsule& r);
	void HitMap(Result_Porygon& r);

private:
	bool m_isActive;
	Collider m_ColliderMap;

	Collider m_Collider;
	SphereInfo m_Sphere;

	RenderTask m_RenderTask;
	Vector3D m_pos;		//位置
	Vector3D m_Oldpos;	//移動前位置（線分判定用）
	Vector3D m_dir;		//方向
	float m_cnt;		//寿命用カウンター
	float m_Time;		//寿命時間
	float m_speed;		//速さ
};

#endif