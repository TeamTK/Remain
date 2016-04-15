#include "Effect.h"

EffectPart::EffectPart(float size, const Vector3D &dir, const Vector3D &pos, const std::string &name) :
	m_Direction(dir),
	m_Pos(pos)
{
	m_Billboard.SetRenderingRegister(true, 10, 0);
	m_Billboard.SetImageAsset(name);
	m_Billboard.SetPosition(m_Pos);
	m_Billboard.SetSize(size);
};

EffectPart::~EffectPart()
{
};

void EffectPart::SetDirection(const Vector3D &direction)
{
	m_Direction = direction;
}

void EffectPart::Update(float speed, float size, int time)
{
	m_Pos += (m_Direction * speed);
	m_Billboard.SetPosition(m_Pos);
	m_Billboard.SetSize(size / time);
}

Effect::Effect(const EffectInfo &info, const char* effectName) :
	m_TimeCnt(1),
	m_AllTime(0),
	m_Speed(1.0f),
	m_Size(1.0f),
	Task(effectName, 0)
{
	m_Speed = info.speed;
	m_Size = info.size;
	m_AllTime = info.time;
};

Effect::~Effect()
{
	auto it = m_list.begin();
	auto itEnd = m_list.end();
	for (; it != itEnd; ) it = m_list.erase(it);
	m_list.clear();
};

void Effect::SetSpeed(float speed)
{
	m_Speed = speed;
}

void Effect::SetSize(float size)
{
	m_Size = size;
}

void Effect::Update()
{
	if (m_TimeCnt > m_AllTime)
	{
		auto it = m_list.begin();
		auto itEnd = m_list.end();
		for (; it != itEnd; ) it = m_list.erase(it);
		m_list.clear();
		Task::SetKill();
	}
	else
	{
		for (auto& i : m_list) i.Update(m_Size, m_Speed, m_TimeCnt);
		m_TimeCnt++;
	}
};