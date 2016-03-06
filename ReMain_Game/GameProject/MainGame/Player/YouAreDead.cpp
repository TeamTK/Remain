#include "YouAreDead.h"
#include "../../Scene/GameOvar.h"
#include "../../Scene/Title.h"
#include "../../Scene/NowLoading.h"

#define ALPHA_SPEED 1.2f
#define DEAD_POS 400, 300
#define RETRY_POS 600, 400
#define QUIT_POS 600, 470

YouAreDead::YouAreDead(int endTime) :
	Task("YouAreDead", 3),
	m_IsDecision(false),
	m_EndTime(endTime),
	m_AlphaCnt(0),
	m_SelectPos(RETRY_POS),
	m_SelectState(SelectState::eRetry)
{
	m_Render.Regist(5, REGIST_RENDER_FUNC(YouAreDead::Render));
	m_Timer.Start();

	//�摜���\�[�X���蓖��
	m_YouAreDeadImage.SetAsset("YouAreDead");
	m_RetryImage.SetAsset("Retry");
	m_QuitImage.SetAsset("Quit");
	m_SelectImage.SetAsset("Blood");

	m_YouAreDeadImage.SetCenter(m_YouAreDeadImage.GetWidth() / 2, m_YouAreDeadImage.GetHeight() / 2);

	//�A���t�@�l0
	m_YouAreDeadImage.SetAlpha(0);
	m_RetryImage.SetAlpha(0);
	m_QuitImage.SetAlpha(0);
	m_SelectImage.SetAlpha(0);

	//�I���摜�ݒ�
	m_SelectImage.SetSize(m_SelectImage.GetWidth() / 4, m_SelectImage.GetHeight() / 5);
	m_SelectImage.SetCenter(m_SelectImage.GetWidth() / 2, m_SelectImage.GetHeight() / 2);

	m_Transfer_In.Start(5);
}

YouAreDead::~YouAreDead()
{
}

void YouAreDead::Update()
{
	if (m_Timer.GetSecond() > m_EndTime)
	{
		Select();

		if (m_IsDecision)
		{
			m_Transfer_In.Update();

			//�t�F�[�h�������I���Ə���
			if (m_Transfer_In.GetIsEndTransfer())
			{
				TaskManager::AllKill();
				SetKill();

				switch (m_SelectState)
				{
				case SelectState::eNo:
					break;

				case SelectState::eRetry:
					new NowLoading(true);
					break;

				case SelectState::eQuit:
					new Title;
					break;
				}
			}
		}
	}
	else
	{
		//�A���t�@�l���Z
		m_AlphaCnt += ALPHA_SPEED;
		m_YouAreDeadImage.SetAlpha((int)m_AlphaCnt);
		m_RetryImage.SetAlpha((int)m_AlphaCnt);
		m_QuitImage.SetAlpha((int)m_AlphaCnt);
		m_SelectImage.SetAlpha((int)m_AlphaCnt);
	}
}

void YouAreDead::Select()
{
	//�I����
	if (Input::KeyUp.Clicked() || Input::XInputPad1.UpClicked())
	{
		m_SelectPos = Vector2D(RETRY_POS);
		m_SelectState = SelectState::eRetry;
	}

	//�I����
	if (Input::KeyDown.Clicked() || Input::XInputPad1.DownClicked())
	{
		m_SelectPos = Vector2D(QUIT_POS);
		m_SelectState = SelectState::eQuit;
	}

	//����{�^��
	if ((Input::KeyEnter.Clicked() || Input::XInputPad1.AClicked()))
	{
		switch (m_SelectState)
		{
		case SelectState::eNo:
			break;

		case SelectState::eRetry:
			m_IsDecision = true;
			break;

		case SelectState::eQuit:
			m_IsDecision = true;
			break;
		}
	}
}

void YouAreDead::Render()
{
	//���肷��Ɖf�点�Ȃ�
	if (!m_IsDecision)
	{
		m_YouAreDeadImage.Draw(DEAD_POS);
		m_RetryImage.Draw(RETRY_POS);
		m_QuitImage.Draw(QUIT_POS);

		m_SelectImage.Draw(m_SelectPos);
	}

	m_Transfer_In.Render();
}