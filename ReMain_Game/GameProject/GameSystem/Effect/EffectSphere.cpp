#include "EffectSphere.h"
#include <random>

EffectSphere::EffectSphere(const EffectInfo &info, const char* effectName) :
	Effect(info, effectName)
{
	Vector3D dir;

	std::random_device rnd;     // �񌈒�I�ȗ���������𐶐�
	std::mt19937 mt(rnd());     //  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l
	std::uniform_real_distribution<> rand(0.0, 6.28);        // [0, 99] �͈͂̈�l����

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