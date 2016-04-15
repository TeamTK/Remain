#include "EffectSphere.h"
#include <random>

EffectSphere::EffectSphere(const EffectInfo &info, const char* effectName) :
	Effect(info, effectName)
{
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

		m_list.emplace_back(info.size, dir, info.pos, info.imageName);
	}
}

EffectSphere::~EffectSphere()
{
}