#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <list>

#include "..\..\GEKO\GEKO.h"

struct EffectInfo
{
	Vector3D pos;
	int num;
	int time;
	float size;
	float speed;
	std::string imageName;
};

//エフェクトの部品
class EffectPart
{
public:
	EffectPart(const Vector3D &dir, const Vector3D &pos, const std::string &name);
	~EffectPart();
	void SetDirection(const Vector3D &direction);
	void Update(float speed, float size, int time);

private:
	Vector3D m_Direction;
	Vector3D m_Pos;
	Billboard m_Billboard;
};

//基底エフェクト
class Effect : public Task
{
public:
	Effect(const EffectInfo &info, const char* effectName);
	virtual ~Effect();
	void SetSpeed(float speed);
	void SetSize(float size);
	virtual void Update();

protected:
	std::list<EffectPart> m_list;

private:
	int m_TimeCnt;
	int m_AllTime;
	float m_Size;
	float m_Speed;
};

#endif