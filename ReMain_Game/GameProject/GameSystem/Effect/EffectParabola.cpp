#include "EffectParabola.h"
#include <random>

EffectParabola::EffectParabola(const EffectInfo &info, const char* effectName, const Vector3D &normal) :
	Effect(info, effectName)
{
	Vector3D dir;

	std::random_device rnd;     // �񌈒�I�ȗ���������𐶐�
	std::mt19937 mt(rnd());     //  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l
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