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
	Fiqure::RenderBillboard(m_Pos, sclae / time, name);
};

Effect::Effect(const EffectInfo &info) :
	m_TimeCnt(0),
	m_AllTime(0),
	m_Speed(1.0f),
	m_Scale(1.0f, 1.0f, 1.0f),
	m_ImageName("NULL")
{
	m_Speed = info.speed;
	m_ImageName = info.imageName;
	m_Scale = info.scale;
	m_AllTime = info.time;
	Vector3D dir;

	std::random_device rnd;     // �񌈒�I�ȗ���������𐶐�
	std::mt19937 mt(rnd());     //  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l
	std::uniform_int_distribution<> rand(0.0, 6.28);        // [0, 99] �͈͂̈�l����

	for (int i = 0; i < info.num; i++)
	{
		dir.x = cosf(rand(mt));
		dir.y = sinf(rand(mt));
		dir.z = sinf(rand(mt));
		dir.SetNormalize();

		m_list.emplace_back(dir.x, dir.y, dir.z, info.pos);
	}
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

bool Effect::IsEnd()
{
	return (m_TimeCnt >= m_AllTime);
}

EffectGeneration::EffectGeneration()
{
}

EffectGeneration::~EffectGeneration()
{
	EffectGeneration* pInstance = EffectGeneration::GetInstance();
	auto it = pInstance->m_list.begin();
	auto itEnd = pInstance->m_list.end();
	for (; it != itEnd;)
	{
		it = pInstance->m_list.erase(it);
	}
	pInstance->m_list.clear();
}

EffectGeneration* EffectGeneration::GetInstance()
{
	static EffectGeneration effectGeneration;
	return &effectGeneration;
}

void EffectGeneration::Add(const EffectInfo &info)
{
	EffectGeneration* pInstance = EffectGeneration::GetInstance();
	pInstance->m_list.emplace_back(info);
}

void EffectGeneration::AllClear()
{
	EffectGeneration* pInstance = EffectGeneration::GetInstance();
	auto it = pInstance->m_list.begin();
	auto itEnd = pInstance->m_list.end();
	for (; it != itEnd;)
	{
		it = pInstance->m_list.erase(it);
	}
	pInstance->m_list.clear();
}

void EffectGeneration::Update()
{
	EffectGeneration* pInstance = EffectGeneration::GetInstance();
	auto it = pInstance->m_list.begin();
	auto itEnd = pInstance->m_list.end();
	for (; it != itEnd;)
	{
		if (it->IsEnd())
		{
			it = pInstance->m_list.erase(it);
		}
		else
		{
			it->Update();
			it++;
		}
	}
}

void EffectGeneration::Render()
{
	EffectGeneration* pInstance = EffectGeneration::GetInstance();
	auto it = pInstance->m_list.begin();
	auto itEnd = pInstance->m_list.end();
	for (; it != itEnd; it++)
	{
		it->Render();
	}
}