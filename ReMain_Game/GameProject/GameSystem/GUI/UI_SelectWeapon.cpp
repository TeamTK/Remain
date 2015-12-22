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
	EWeapons stage;
};

static SData WeaponData[] =
{
	{ 172.0f, 140.0f, eHand		},
	{ 200.0f, 320.0f, eShotgun	},
	{ 370.0f, 180.0f, eHandgun	},
	{ 370.0f, 180.0f, eNone		},
};

#define MAX_SIZE 512


SelectWeapon::SelectWeapon() :
	m_isActive(false), m_Size(0)
{
	State = eOpen;
	m_Image.SetAsset("SelectWeapon");
	m_Image.SetCenter(256, 256);
	m_Pos = Vector2D(400.0f, 300.0f);
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

			break;
		case eSelect:




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
	}

	m_Image.SetCenter(m_Size/2, m_Size / 2);
	m_Image.Draw(m_Pos.x, m_Pos.y);
}

EWeapons SelectWeapon::Select()
{
	m_isActive = !m_isActive;
	return m_Weapon;
}

void SelectWeapon::Draw()
{

}
