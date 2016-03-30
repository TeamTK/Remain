#include "Title.h"
#include "NowLoading.h"

#define TITLE_POS 100, 200
#define START_POS 600, 350
#define EXIT_POS 600, 420

bool g_isResource = false;

Title::Title() :
	Task("Title", 0),
	m_IsTransferStart(false),
	m_IsTransferEnd(false),
	m_SelectPos(START_POS),
	m_Select(Select::eStart)
{
	//リソース設定2D
	m_TitleImage.SetAsset("Remain");
	m_StartImage.SetAsset("Start");
	m_ExitImage.SetAsset("Exit");
	m_SelectImage.SetAsset("Blood");

	m_TitleImage.SetDrawRegister(true, 0, 0);
	m_StartImage.SetDrawRegister(true, 0, 1);
	m_ExitImage.SetDrawRegister(true, 0, 2);
	m_SelectImage.SetDrawRegister(true, 0, 3);

	m_TitleImage.SetPosition(TITLE_POS);
	m_StartImage.SetPosition(START_POS);
	m_ExitImage.SetPosition(EXIT_POS);
	m_SelectImage.SetPosition(m_SelectPos);

	//リソース設定3D
	m_Cabin.SetAsset("Cabin");
	m_ShotGun.SetAsset("Shotgun");
	m_HandGun.SetAsset("Handgun");
	m_AmmoBox.SetAsset("AmmoBox");

	m_Cabin.SetRenderingRegister(true, 0, 0);
	m_ShotGun.SetRenderingRegister(true, 0, 1);
	m_HandGun.SetRenderingRegister(true, 0, 2);
	m_AmmoBox.SetRenderingRegister(true, 0, 3);

	//リソース設定サウンド
	m_DecisionSound.SetAseet("Decision");
	m_CursorSound.SetAseet("Cursor");

	//選択画像設定
	m_SelectImage.SetSize(m_SelectImage.GetWidth() / 4, m_SelectImage.GetHeight() / 5);
	m_SelectImage.SetCenter(m_SelectImage.GetWidth() / 2, m_SelectImage.GetHeight() / 2);

	//3Dオブジェクト回転と平行移動
	m_ShotGun.SetRotationDegree(240, 0, 90);
	m_HandGun.SetRotationDegree(220, 0, 90);
	m_AmmoBox.SetRotationDegree(0, 200, 0);
	m_ShotGun.SetTranselate(-1.5f, 1.0f, -3.0f);
	m_HandGun.SetTranselate(-1.5f, 1.0f, -2.8f);
	m_AmmoBox.SetTranselate(-1.7f, 1.0f, -3.1f);
	m_AmmoBox.SetScale(0.5f, 0.5f, 0.5f);

	//カメラ位置と注視点
	Camera::SetEye(-1.5f, 2.0f, -2.0f);
	Camera::SetLookat(-1.0f, 0.0f, -4.0f);

	m_Transfer_Out.Start(4.0f);
}

Title::~Title()
{
}

void Title::Update()
{
	m_Transfer_Out.Update();
	if (!m_Transfer_Out.GetIsEndTransfer()) return;

	//選択上
	if (Input::KeyUp.Clicked() || Input::XInputPad1.UpClicked())
	{
		m_CursorSound.Play();
		m_SelectPos = Vector2D(START_POS);
		m_Select = Select::eStart;
	}

	//選択下
	if (Input::KeyDown.Clicked() || Input::XInputPad1.DownClicked())
	{
		m_CursorSound.Play();
		m_SelectPos = Vector2D(EXIT_POS);
		m_Select = Select::eExit;
	}

	//決定ボタン
	if ((Input::KeyEnter.Clicked() || Input::XInputPad1.AClicked()) && !m_IsTransferStart)
	{
		switch (m_Select)
		{
		case Select::eNo:
			break;

		case Select::eStart:
			m_DecisionSound.Play();
			m_Transfer.Start(4.0f);
			m_IsTransferStart = true;
			break;

		case Select::eExit:
			SetKill();
			GEKO::LoopEnd();

			break;
		}
	}

	if (m_Transfer.GetIsEndTransfer() && !m_IsTransferEnd)
	{
		m_IsTransferEnd = true;

		m_TitleImage.SetDrawRegister(false, 0, 0);
		m_StartImage.SetDrawRegister(false, 0, 1);
		m_ExitImage.SetDrawRegister(false, 0, 2);
		m_SelectImage.SetDrawRegister(false, 0, 3);

		m_Cabin.SetRenderingRegister(false, 0, 0);
		m_ShotGun.SetRenderingRegister(false, 0, 1);
		m_HandGun.SetRenderingRegister(false, 0, 2);
		m_AmmoBox.SetRenderingRegister(false, 0, 3);

		SetKill();
		new NowLoading(ChapterType::eChapter_1_1, g_isResource);
		g_isResource = true;
	}

	m_Transfer.Update();
	m_SelectImage.SetPosition(m_SelectPos);
}