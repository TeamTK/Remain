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

	m_Render.Regist(6, REGIST_RENDER_FUNC(Menu::Render));

	//画像リソース割り当て
	m_RetryImage.SetAsset("Retry");
	m_QuitImage.SetAsset("Quit");
	m_SelectImage.SetAsset("Blood");
	m_BackgroundImage.SetAsset("Transfer_Black");

	m_BackgroundImage.SetAlpha(100);

	//選択画像設定
	m_SelectImage.SetSize(m_SelectImage.GetWidth() / 4, m_SelectImage.GetHeight() / 5);
	m_SelectImage.SetCenter(m_SelectImage.GetWidth() / 2, m_SelectImage.GetHeight() / 2);

	m_Transfer_In.Start(5);
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
				new NowLoading(true);
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

void Menu::Render()
{
	//決定すると映らせない
	if (!m_IsDecision)
	{
		m_RetryImage.Draw(MENU_RETRY_POS);
		m_QuitImage.Draw(MENU_QUIT_POS);

		m_SelectImage.Draw(m_SelectPos);
		m_BackgroundImage.Draw(0, 0);
	}

	m_Transfer_In.Render();
}

void Menu::Select()
{
	if (Input::KeyEscape.Clicked() && !m_IsStop)
	{
		TaskManager::AllStart();
		SetKill();
		return;
	}
	m_IsStop = false;

	//選択上
	if (Input::KeyUp.Clicked() || Input::XInputPad1.UpClicked())
	{
		m_SelectPos = Vector2D(MENU_RETRY_POS);
		m_SelectState = SelectState::eRetry;
	}

	//選択下
	if (Input::KeyDown.Clicked() || Input::XInputPad1.DownClicked())
	{
		m_SelectPos = Vector2D(MENU_QUIT_POS);
		m_SelectState = SelectState::eQuit;
	}

	//決定ボタン
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