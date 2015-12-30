#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <list>
#include <random>

#include "..\GEKO\Figure\Fiqure.h"

struct EffectInfo
{
	Vector3D pos;
	Vector3D scale;
	int num;
	int time; 
	float speed; 
	std::string imageName;
};

class EffectPart
{
public:
	EffectPart(float x, float y, float z, const Vector3D &pos);
	~EffectPart();
	void SetDirection(const Vector3D &direction);
	void Render(const Vector3D &sclae, float speed, int time, const std::string &name);

private:
	Vector3D m_Direction;
	Vector3D m_Pos;
};

class Effect
{
public:
	Effect(const EffectInfo &info);
	~Effect();
	void SetSpeed(float speed);
	void SetScale(float x, float y, float z);
	void Update();
	void Render();
	bool IsEnd();

private:
	int m_TimeCnt;
	int m_AllTime;
	float m_Speed;
	Vector3D m_Scale;
	std::string m_ImageName;
	std::list<EffectPart> m_list;
};

class EffectGeneration
{
public:
	~EffectGeneration();
	static void Add(const EffectInfo &info);
	static void AllClear();
	static void Update();
	static void Render();

private:
	EffectGeneration();
	static EffectGeneration* GetInstance();

private:
	std::list<Effect> m_list;
};

#endif