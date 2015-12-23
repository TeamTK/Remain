#include "UI_SelectWeapon.h"

enum EState
{
	eOpen,
	eSelect,
} State;

struct SData
{
	float x;
	float y;
	EWeapons weapons;
};

static SData WeaponData[] =
{
	{ 400.0f, 445.0f, eHand		},	//Down
	{ 400.0f, 155.0f, eShotgun	},	//Up
	{ 255.0f, 300.0f, eHandgun	},	//Left
	{ 545.0f, 300.0f, eNone		},	//Right
};

#define MAX_SIZE 512

UI_SelectWeapon::UI_SelectWeapon() :
	m_isSelected(false), m_Size(0), m_Selected(1),
	Task("UI_SelectWeapon", 1)
{
	m_RenderTask.Regist(1, REGIST_RENDER_FUNC(UI_SelectWeapon::Draw));
	State = eOpen;
	m_Image.SetAsset("SelectWeapon");
	m_SelectedCursor.SetAsset("SelectedCursor");
	m_SelectedCursor.SetSize(256, 256);
	m_SelectedCursor.SetCenter(128, 128);
	m_ScPos = Vector2D(400.0f, 155.0f);
}

UI_SelectWeapon::~UI_SelectWeapon()
{

}

void UI_SelectWeapon::Update()
{
	if (m_isSelected)
	{
		switch (State)
		{
		case eOpen:
			if (m_Size < MAX_SIZE) m_Size += 64;
			m_Image.SetSize(m_Size, m_Size);
			if (m_Size >= MAX_SIZE)	State = eSelect;

			break;
		case eSelect:
			//武器選択(方向キー、十字キー)
			if (Input::KeyDown.Clicked() || Input::XInputPad1.DownClicked())	m_Selected = 0;
			if (Input::KeyUp.Clicked() || Input::XInputPad1.UpClicked())		m_Selected = 1;
			if (Input::KeyLeft.Clicked() || Input::XInputPad1.LeftClicked())	m_Selected = 2;
			if (Input::KeyRight.Clicked() || Input::XInputPad1.RightClicked())	m_Selected = 3;

			//カーソル移動
			m_ScPos.x = Vector3D::Lerp(Vector3D(m_ScPos.x, 0.0f, 0.0f), Vector3D(WeaponData[m_Selected].x, 0.0f, 0.0f), 0.8f).x;
			m_ScPos.y = Vector3D::Lerp(Vector3D(0.0f, m_ScPos.y, 0.0f), Vector3D(0.0f, WeaponData[m_Selected].y, 0.0f), 0.8f).y;

			break;
		default:
			break;
		}
	}
	else
	{
		if (m_Size > 0) m_Size -= 64;
		m_Image.SetSize(m_Size, m_Size);
		State = eOpen;
	}
}

EWeapons UI_SelectWeapon::Select()
{
	m_isSelected = !m_isSelected;
	return WeaponData[m_Selected].weapons;
}

void UI_SelectWeapon::Draw()
{
	if (State == eSelect)
		m_SelectedCursor.Draw((int)m_ScPos.x, (int)m_ScPos.y);

	m_Image.SetCenter(m_Size / 2, m_Size / 2);
	m_Image.Draw(400, 300);
}

bool UI_SelectWeapon::GetisSelected()
{
	return m_isSelected;
}