#ifndef _CHAPTER_H_
#define _CHAPTER_H_

#include "../GEKO/GEKO.h"
#include "../GameSystem/ScreenTransfer.h"

class Chapter_1_1 : public Task
{
public:
	Chapter_1_1();
	~Chapter_1_1();
	void Update();
	void Render();
	void StageChange(Result_Sphere &data);

private:
	float m_Radius;
	Collider m_StageChange;	//ステージ移動用
	Vector3D m_pos;
	Vector3D m_StageChangePos;	//ステージ移動用当たり判定座標
	Sound m_BGM;
	ScreenTransfer_In  m_Transfer_In;
	RenderTask m_Render;
};


class Chapter_1_2 : public Task
{
public:
	Chapter_1_2();
	~Chapter_1_2();
	void Update();
	void Render();

	void StageChange(Result_Sphere &data);

private:
	float m_Radius;
	Collider m_StageChange;	//ステージ移動用
	Vector3D m_pos;
	Vector3D m_StageChangePos;	//ステージ移動用当たり判定座標
	ScreenTransfer_In  m_Transfer_In;
	Sound m_BGM;
	RenderTask m_Render;
};


class Chapter_1_3 : public Task
{
public:
	Chapter_1_3();
	~Chapter_1_3();
	void Update();
	void Render();

	void HitPlayer(Result_Sphere &data);

private:
	float m_Radius;
	Collider m_MapCol;
	Vector3D m_pos;
	Sound m_BGM;
	ScreenTransfer_In  m_Transfer_In;
	RenderTask m_Render;
};
#endif