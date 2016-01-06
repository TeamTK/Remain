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
	Collider m_ColliderMap; //�}�b�v�Ƃ̓����蔻��
	Collider m_Collider; //�L�����N�^�Ƃ̓����蔻��
	Vector3D m_Pos;		//�ʒu
	Vector3D m_Oldpos;	//�ړ��O�ʒu�i��������p�j
	Vector3D m_Dir;		//����
	float m_Cnt;		//�����p�J�E���^�[
	float m_Time;		//��������
	float m_Speed;		//����
	float m_Radius;
};

#endif