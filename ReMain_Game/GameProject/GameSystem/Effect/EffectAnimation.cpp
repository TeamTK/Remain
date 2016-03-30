#include "EffectAnimation.h"

EffectAnimation::EffectAnimation(const std::string &assetName, const EffectAnimationInfo &info) :
	Task(assetName.c_str(), 2),
	m_Pos(info.pos),
	m_Size(info.size),
	m_Speed(info.speed)
{
	m_Anim.SetRenderingRegister(true, 10, 0);
	m_Anim.FrameDivision(assetName, info.frameNum, info.sizeW, info.sizeH);
}

EffectAnimation::~EffectAnimation()
{
}

void EffectAnimation::Update()
{
	if (m_Anim.GetIsEnd())
	{
		SetKill();
	}
	m_Anim.PlayFrame(m_Speed);
	m_Anim.SetPosition(m_Pos);
	m_Anim.SetSize(m_Size);
}