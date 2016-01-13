#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <list>

#include "..\..\GEKO\Figure\Fiqure.h"
#include "..\..\GEKO\Task\RenderTask.h"

struct EffectInfo
{
	Vector3D pos;
	Vector3D scale;
	int num;
	int time;
	float speed;
	std::string imageName;
};

//エフェクトの部品
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

//基底エフェクト
class Effect : public Task
{
public:
	Effect(const EffectInfo &info, const char* effectName);
	virtual ~Effect();
	void SetSpeed(float speed);
	void SetScale(float x, float y, float z);
	virtual void Update();
	void Render();

protected:
	std::list<EffectPart> m_list;

private:
	int m_TimeCnt;
	int m_AllTime;
	float m_Speed;
	Vector3D m_Scale;
	std::string m_ImageName;
	RenderTask m_RenderTask;
};

#endif