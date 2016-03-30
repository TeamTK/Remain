#ifndef _CHAPTER_H_
#define _CHAPTER_H_

#include "../GameSystem/ScreenTransfer.h"

enum ChapterType
{
	eChapter_1_1,
	eChapter_1_2,
	eChapter_1_3,
};

extern ChapterType g_ChapterType;

class Chapter_1_1 : public Task
{
public:
	Chapter_1_1();
	~Chapter_1_1();
	void Update();
	void StageChange(Result_Sphere &data);

private:
	float m_Radius;
	StaticMesh m_TracerouteMesh;
	Collider m_StageChange;	//ステージ移動用
	Vector3D m_StageChangePos;	//ステージ移動用当たり判定座標
	Sound m_BGM;
	ScreenTransfer_In m_Transfer_In;
	ScreenTransfer_Out m_Transfer_Out;
};

class Chapter_1_2 : public Task
{
public:
	Chapter_1_2();
	~Chapter_1_2();
	void Update();
	void StageChange(Result_Sphere &data);

private:
	float m_Radius;
	StaticMesh m_TracerouteMesh;
	Collider m_StageChange;	//ステージ移動用
	Vector3D m_StageChangePos;	//ステージ移動用当たり判定座標
	ScreenTransfer_In m_Transfer_In;
	ScreenTransfer_Out m_Transfer_Out;
	Sound m_BGM;
};


class Chapter_1_3 : public Task
{
public:
	Chapter_1_3();
	~Chapter_1_3();
	void Update();

private:
	Sound m_BGM;
	ScreenTransfer_In m_Transfer_In;
	ScreenTransfer_Out m_Transfer_Out;
};
#endif