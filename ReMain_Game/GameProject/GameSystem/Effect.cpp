#include "Effect.h"
#include <random>

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
	Vector3D dir;

	std::random_device rnd;     // 非決定的な乱数生成器を生成
	std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_real_distribution<> rand(0.0, 6.28);        // [0, 99] 範囲の一様乱数

	for (int i = 0; i < info.num; i++)
	{
		dir.x = cosf((float)rand(mt));
		dir.y = sinf((float)rand(mt));
		dir.z = sinf((float)rand(mt));
		dir.SetNormalize();

		m_list.emplace_back(dir.x, dir.y, dir.z, info.pos);
	}
	m_RenderTask.Regist(0, REGIST_RENDER_FUNC(Effect::Render));
};

Effect::~Effect()
{
	auto it = m_list.begin();
	auto itEnd = m_list.end();
	for (; it != itEnd; )
	{
		it = m_list.erase(it);
	}
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
		for (; it != itEnd; )
		{
			it = m_list.erase(it);
		}
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
	auto it = m_list.begin();
	auto itEnd = m_list.end();
	for (; it != itEnd; it++)
	{
		it->Render(m_Scale, m_Speed, m_TimeCnt, m_ImageName);
	}
};