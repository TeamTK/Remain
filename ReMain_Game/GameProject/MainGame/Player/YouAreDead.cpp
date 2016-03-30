#include "YouAreDead.h"
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
	m_Timer.Start();

	//�摜���\�[�X���蓖��
	m_YouAreDeadImage.SetAsset("YouAreDead");
	m_RetryImage.SetAsset("Retry");
	m_QuitImage.SetAsset("Quit");
	m_SelectImage.SetAsset("Blood");

	//�`��o�^
	m_YouAreDeadImage.SetDrawRegister(true, 5, 0);
	m_RetryImage.SetDrawRegister(true, 5, 1);
	m_QuitImage.SetDrawRegister(true, 5, 2);
	m_SelectImage.SetDrawRegister(true, 5, 3);

	//�ʒu
	m_YouAreDeadImage.SetPosition(DEAD_POS);
	m_RetryImage.SetPosition(RETRY_POS);
	m_QuitImage.SetPosition(QUIT_POS);
	m_SelectImage.SetPosition(m_SelectPos);

	m_YouAreDeadImage.SetCenter(m_YouAreDeadImage.GetWidth() / 2, m_YouAreDeadImage.GetHeight() / 2);

	//���y���\�[�X���蓖��
	m_YouAreDeadSound.SetAseet("YouAreDead");
	m_DecisionSound.SetAseet("Decision");
	m_CursorSound.SetAseet("Cursor");

	//�A���t�@�l0
	m_YouAreDeadImage.SetAlpha(0);
	m_RetryImage.SetAlpha(0);
	m_QuitImage.SetAlpha(0);
	m_SelectImage.SetAlpha(0);

	//�I���摜�ݒ�
	m_SelectImage.SetSize(m_SelectImage.GetWidth() / 4, m_SelectImage.GetHeight() / 5);
	m_SelectImage.SetCenter(m_SelectImage.GetWidth() / 2, m_SelectImage.GetHeight() / 2);

	m_Transfer_In.Start(5);

	m_YouAreDeadSound.Play();
}

YouAreDead::~YouAreDead()
{
}

void YouAreDead::Update()
{
	if (m_Timer.GetSecond() > m_EndTime)
	{
		if (m_IsDecision)
		{
			m_Transfer_In.Update();

			//�t�F�[�h�������I���Ə���
			if (m_Transfer_In.GetIsEndTransfer())
			{
				TaskManager::AllKill();

				switch (m_SelectState)
				{
				case SelectState::eNo:
					break;

				case SelectState::eRetry:
					new NowLoading(g_ChapterType, true);
					break;

				case SelectState::eQuit:
					new Title;
					break;
				}
			}
		}
		else
		{
			Select();
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
		m_CursorSound.Play();
		m_SelectPos = Vector2D(RETRY_POS);
		m_SelectState = SelectState::eRetry;
	}

	//�I����
	if (Input::KeyDown.Clicked() || Input::XInputPad1.DownClicked())
	{
		m_CursorSound.Play();
		m_SelectPos = Vector2D(QUIT_POS);
		m_SelectState = SelectState::eQuit;
	}

	//����{�^��
	if ((Input::KeyEnter.Clicked() || Input::XInputPad1.AClicked()))
	{
		m_DecisionSound.Play();

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
	m_SelectImage.SetPosition(m_SelectPos);
}