#include "ScreenRecovery.h"

#define RECOVERY_ALPHA_FADE_IN_SPEED 4.0f
#define RECOVERY_ALPHA_FADE_OUT_SPEED 4.0f
#define RECOVERY_ALPHA_MAX 60

ScreenRecovery::ScreenRecovery() :
	Task("ScreenRecovery", 0),
	m_IsFadeOut(false),
	m_Alpha(0.0f)
{
	m_Render.Regist(6, REGIST_RENDER_FUNC(ScreenRecovery::Draw));
	m_RecoveryImage.SetAsset("ScreenRecovery");
	m_RecoveryImage.SetAlpha(0);
}

ScreenRecovery::~ScreenRecovery()
{
}

void ScreenRecovery::Update()
{
	//�t�F�[�h�X�V
	if (m_IsFadeOut)
	{
		//�t�F�[�h�A�E�g
		if (m_Alpha < 0)
		{
			Task::SetKill();
		}
		m_Alpha -= RECOVERY_ALPHA_FADE_OUT_SPEED;
	}
	else
	{
		//�t�F�[�h�C��
		if (m_Alpha > RECOVERY_ALPHA_MAX)
		{
			m_IsFadeOut = true;
		}
		m_Alpha += RECOVERY_ALPHA_FADE_IN_SPEED;
	}
	m_RecoveryImage.SetAlpha((int)m_Alpha);
}

void ScreenRecovery::Draw()
{
	m_RecoveryImage.Draw(0, 0);
}