#ifndef _CHAPTER_H_
#define _CHAPTER_H_

#include "../GEKO/GEKO.h"

class Chapter_1_1 : public Task
{
public:
	Chapter_1_1();
	~Chapter_1_1();
	void Update();

	void HitPlayer(Result_Sphere &data);
	void StageChange(Result_Sphere &data);

private:
	float m_Radius;
	bool m_isHit;
	Collider m_MapCol;
	Collider m_StageChange;	//�X�e�[�W�ړ��p
	Vector3D m_pos;
	Vector3D m_StageChangePos;	//�X�e�[�W�ړ��p�����蔻����W
	Sound m_BGM;
};



class Chapter_1_2 : public Task
{
public:
	Chapter_1_2();
	~Chapter_1_2();
	void Update();

	void HitPlayer(Result_Sphere &data);

private:
	float m_Radius;
	bool m_isHit;
	Collider m_MapCol;
	Vector3D m_pos;
	Sound m_BGM;
};

#endif