#include "EffectParabola.h"
#include <random>

EffectParabola::EffectParabola(const EffectInfo &info, const char* effectName, const Vector3D &normal) :
	Effect(info, effectName)
{
	Vector3D dir;

	std::random_device rnd;     // 非決定的な乱数生成器を生成
	std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_real_distribution<> rand(0.0, 6.28); 

	for (int i = 0; i < info.num; i++)
	{
		dir.x = cosf((float)rand(mt));
		dir.y = sinf((float)rand(mt));
		dir.z = sinf((float)rand(mt));
		dir += normal;
		dir.SetNormalize();

		m_list.emplace_back(dir.x, dir.y, dir.z, info.pos);
	}
}

EffectParabola::~EffectParabola()
{
}

void EffectParabola::Update()
{
	Effect::Update();
}