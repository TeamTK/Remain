#include "UI_SelectWeapon.h"

enum EState
{
	eOpen,
	eSelect,
	eClose
} State;


struct SData
{
	float x;
	float y;
	EWeapons weapons;
};

static SData WeaponData[] =
{
	{ 400.0f, 535.0f, eHand		},
	{ 400.0f, 75.0f, eShotgun	},
	{ 175.0f, 300.0f, eHandgun	},
	{ 625.0f, 300.0f, eNone		},
};

#define MAX_SIZE 512


SelectWeapon::SelectWeapon() :
	m_isActive(false), m_Size(0), m_Selected(0)
{
	State = eOpen;
	m_Image.SetAsset("SelectWeapon");
	m_SelectedCircle = m_Image;
	m_SelectedCircle.SetSize(256, 256);
	m_SelectedCircle.SetCenter(128, 1280);
	m_Pos = Vector2D(400.0f, 300.0f);
	m_ScPos = Vector2D(400.0f, 75.0f);
}

SelectWeapon::~SelectWeapon()
{

}

void SelectWeapon::Update()
{
	if (m_isActive)
	{
		switch (State)
		{
		case eOpen:
			if (m_Size < MAX_SIZE) m_Size += 64;
			m_Image.SetSize(m_Size, m_Size);
			if (m_Size >= MAX_SIZE)	State = eSelect;

			break;
		case eSelect:
			if (Input::KeyDown.Clicked() || Input::XInputPad1.DownClicked())	m_Selected = 0;
			if (Input::KeyUp.Clicked() ||	Input::XInputPad1.UpClicked())		m_Selected = 1;
			if (Input::KeyLeft.Clicked() || Input::XInputPad1.LeftClicked())	m_Selected = 2;
			if (Input::KeyRight.Clicked() || Input::XInputPad1.RightClicked())	m_Selected = 3;

			
			m_ScPos.x = Vector3D::Lerp(Vector3D(m_ScPos.x, 0.0f, 0.0f), Vector3D(WeaponData[m_Selected].x, 0.0f, 0.0f), 0.8).x;
			m_ScPos.y = Vector3D::Lerp(Vector3D(0.0f, m_ScPos.y, 0.0f), Vector3D(0.0f, WeaponData[m_Selected].y, 0.0f), 0.8).y;

			m_SelectedCircle.Draw(m_ScPos.x, m_ScPos.y);

			break;
		case eClose:

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

	m_Image.SetCenter(m_Size / 2, m_Size / 2);
	m_Image.Draw(m_Pos.x, m_Pos.y);
	m_ScPos.DebugDraw("m_ScPos");
}

EWeapons SelectWeapon::Select()
{
	m_isActive = !m_isActive;
	return WeaponData[m_Selected].weapons;
}

void SelectWeapon::Draw()
{

}
