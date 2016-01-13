#include "Effect.h"

EffectPart::EffectPart(float x, float y, float z, const Vector3D &pos)
{
	m_Direction.x = x;
	m_Direction.y = y;
	m_Direction.z = z;

	m_Pos = pos;
};

EffectPart::~EffectPart()
{
};

void EffectPart::SetDirection(const Vector3D &direction)
{
	m_Direction = direction;
}

void EffectPart::Render(const Vector3D &sclae, float speed, int time, const std::string &name)
{
	m_Pos += (m_Direction * speed);
	Fiqure::RenderBillboard(m_Pos, sclae / (float)time, name);
};

Effect::Effect(const EffectInfo &info, const char* effectName) :
	m_TimeCnt(0),
	m_AllTime(0),
	m_Speed(1.0f),
	m_Scale(1.0f, 1.0f, 1.0f),
	m_ImageName("NULL"),
	Task(effectName, 0)
{
	m_Speed = info.speed;
	m_ImageName = info.imageName;
	m_Scale = info.scale;
	m_AllTime = info.time;

	m_RenderTask.Regist(0, REGIST_RENDER_FUNC(Effect::Render));
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

void Effect::SetScale(float x, float y, float z)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
}

void Effect::Update()
{
	if (m_TimeCnt >= m_AllTime)
	{
		auto it = m_list.begin();
		auto itEnd = m_list.end();
		for (; it != itEnd; ) it = m_list.erase(it);
		m_list.clear();
		Task::SetKill();
	}
	else
	{
		m_TimeCnt++;
	}
};

void Effect::Render()
{
	for (auto& i : m_list) i.Render(m_Scale, m_Speed, m_TimeCnt, m_ImageName);
};