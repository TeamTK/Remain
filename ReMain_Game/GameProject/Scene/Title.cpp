#include "Title.h"
#include "NowLoading.h"

#define TITLE_POS 100, 200
#define START_POS 600, 350
#define EXIT_POS 600, 420

bool g_isResource = false;

Title::Title() :
	Task("Title", 0),
	m_SelectPos(-100, -100),
	m_Select(Select::eNo)
{
	//���\�[�X�ݒ�2D
	m_TitleImage.SetAsset("Remain");
	m_StartImage.SetAsset("Start");
	m_ExitImage.SetAsset("Exit");
	m_SelectImage.SetAsset("Blood");

	//���\�[�X�ݒ�3D
	m_Cabin.SetAsset("Cabin");
	m_ShotGun.SetAsset("Shotgun");
	m_HandGun.SetAsset("Handgun");

	//�I���摜�ݒ�
	m_SelectImage.SetSize(m_SelectImage.GetWidth() / 4, m_SelectImage.GetHeight() / 5);
	m_SelectImage.SetCenter(m_SelectImage.GetWidth() / 2, m_SelectImage.GetHeight() / 2);

	//3D�I�u�W�F�N�g��]�ƕ��s�ړ�
	m_ShotGun.SetRotationDegree(240, 0, 90);
	m_HandGun.SetRotationDegree(220, 0, 90);
	m_ShotGun.SetTranselate(-1.5f, 1.0f, -3.0f);
	m_HandGun.SetTranselate(-1.5f, 1.0f, -2.8f);

	m_Render.Regist(0, REGIST_RENDER_FUNC(Title::Render));

	//�J�����ʒu�ƒ����_
	Camera::SetEye(-1.5f, 2.0f, -2.0f);
	Camera::SetLookat(-1.0f, 0.0f, -4.0f);
}

Title::~Title()
{
}

void Title::Update()
{
	//�I����
	if (Input::KeyUp.Clicked() || Input::XInputPad1.UpClicked())
	{
		m_SelectPos = Vector2D(START_POS);
		m_Select = Select::eStart;
	}

	//�I����
	if (Input::KeyDown.Clicked() || Input::XInputPad1.DownClicked())
	{
		m_SelectPos = Vector2D(EXIT_POS);
		m_Select = Select::eExit;
	}

	//����{�^��
	if (Input::KeyEnter.Clicked() || Input::XInputPad1.AClicked())
	{
		switch (m_Select)
		{
		case Select::eNo:
			break;

		case Select::eStart:
			SetKill();
			m_Render.Sleep();
			new NowLoading(g_isResource);
			g_isResource = true;

			break;

		case Select::eExit:
			SetKill();
			m_Render.Sleep();
			GEKO::LoopEnd();

			break;
		}
	}
}

void Title::Render()
{
	//3D�I�u�W�F�N�g
	m_Cabin.Render();
	m_ShotGun.Render();
	m_HandGun.Render();

	//2D�I�u�W�F�N�g
	m_TitleImage.Draw(TITLE_POS);
	m_StartImage.Draw(START_POS);
	m_ExitImage.Draw(EXIT_POS);
	m_SelectImage.Draw(m_SelectPos);
}