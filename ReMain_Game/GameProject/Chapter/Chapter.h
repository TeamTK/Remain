#ifndef _CHAPTER_H_
#define _CHAPTER_H_

#include "..\GEKO\Task\Task.h"
#include "../GEKO/GEKO.h"

class Chapter_1_1 : public Task
{
public:
	Chapter_1_1();
	~Chapter_1_1();
	void Update();

	void HitPlayer(Result_Sphere &data);

private:
	float m_Radius;
	bool m_isHit;
	Collider m_MapCol;
	Vector3D m_pos;
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
};

#endif