#include "EffectAnimation.h"

EffectAnimation::EffectAnimation(const std::string &assetName, const EffectAnimationInfo &info) :
	Task(assetName.c_str(), 2),
	m_Pos(info.pos),
	m_Size(info.size),
	m_Speed(info.speed)
{
	m_Anim.FrameDivision(assetName, info.frameNum, info.sizeW, info.sizeH);
	m_Render.Regist(0, REGIST_RENDER_FUNC(EffectAnimation::Render));
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
}

void EffectAnimation::Render()
{
	m_Anim.Render(m_Pos, m_Size);
}