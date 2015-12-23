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
	Vector3D m_pos;		//�ʒu
	Vector3D m_Oldpos;	//�ړ��O�ʒu�i��������p�j
	Vector3D m_dir;		//����
	float m_cnt;		//�����p�J�E���^�[
	float m_Time;		//��������
	float m_speed;		//����
};

#endif