#include "Menu.h"
#include "../../Scene/Title.h"
#include "../../Scene/NowLoading.h"

#define MENU_RETRY_POS 600, 400
#define MENU_QUIT_POS 600, 470

Menu::Menu() :
	Task("Menu", 0),
	m_IsStop(true),
	m_IsDecision(false),
	m_SelectPos(MENU_RETRY_POS),
	m_SelectState(SelectState::eRetry)
{
	TaskManager::AllStop();
	TaskManager::Start("Gravity");
	TaskManager::Start("Menu");

	//画像リソース割り当て
	m_RetryImage.SetAsset("Retry");
	m_QuitImage.SetAsset("Quit");
	m_SelectImage.SetAsset("Blood");
	m_BackgroundImage.SetAsset("Transfer_Black");

	//描画登録
	m_RetryImage.SetDrawRegister(true, 6, 0);
	m_QuitImage.SetDrawRegister(true, 6, 1);
	m_SelectImage.SetDrawRegister(true, 6, 2);
	m_BackgroundImage.SetDrawRegister(true, 6, 3);

	//位置を設定
	m_RetryImage.SetPosition(MENU_RETRY_POS);
	m_QuitImage.SetPosition(MENU_QUIT_POS);
	m_SelectImage.SetPosition(m_SelectPos);
	m_BackgroundImage.SetPosition(0, 0);

	m_BackgroundImage.SetAlpha(100);

	//音楽リソース割り当て
	m_OpenSound.SetAseet("MenuOpen");
	m_DecisionSound.SetAseet("Decision");
	m_CursorSound.SetAseet("Cursor");

	//選択画像設定
	m_SelectImage.SetSize(m_SelectImage.GetWidth() / 4, m_SelectImage.GetHeight() / 5);
	m_SelectImage.SetCenter(m_SelectImage.GetWidth() / 2, m_SelectImage.GetHeight() / 2);

	m_Transfer_In.Start(5);

	m_OpenSound.Play();
}

Menu::~Menu()
{

}

void Menu::Update()
{
	if (m_IsDecision)
	{
		m_Transfer_In.Update();

		//フェード処理が終わると消す
		if (m_Transfer_In.GetIsEndTransfer())
		{
			TaskManager::AllKill();
			SetKill();

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

void Menu::Select()
{
	if ((Input::KeyEscape.Clicked() || Input::XInputPad1.StartClicked()) && !m_IsStop)
	{
		TaskManager::AllStart();
		SetKill();
		return;
	}
	m_IsStop = false;

	//選択上
	if (Input::KeyUp.Clicked() || Input::XInputPad1.UpClicked())
	{
		m_CursorSound.Play();
		m_SelectPos = Vector2D(MENU_RETRY_POS);
		m_SelectState = SelectState::eRetry;
	}

	//選択下
	if (Input::KeyDown.Clicked() || Input::XInputPad1.DownClicked())
	{
		m_CursorSound.Play();
		m_SelectPos = Vector2D(MENU_QUIT_POS);
		m_SelectState = SelectState::eQuit;
	}

	//決定ボタン
	if ((Input::KeyEnter.Clicked() || Input::XInputPad1.AClicked()))
	{
		m_DecisionSound.Play();

		switch (m_SelectState)
		{
		case SelectState::eNo:
			break;

		case SelectState::eRetry:
			m_IsDecision = true;
			StopRendering();
			break;

		case SelectState::eQuit:
			m_IsDecision = true;
			StopRendering();
			break;
		}
	}
	m_SelectImage.SetPosition(m_SelectPos);
}

void Menu::StopRendering()
{
	//描画登録
	m_RetryImage.SetDrawRegister(false, 6, 0);
	m_QuitImage.SetDrawRegister(false, 6, 1);
	m_SelectImage.SetDrawRegister(false, 6, 2);
	m_BackgroundImage.SetDrawRegister(false, 6, 3);
}