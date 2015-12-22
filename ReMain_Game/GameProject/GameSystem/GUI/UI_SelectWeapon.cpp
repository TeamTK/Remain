#include "UI_SelectWeapon.h"

enum EState
{
	eOpen,
	eSelect,
	eClose
} State;

SelectWeapon::SelectWeapon() :
	m_isActive(false), m_Size(0)
{
	State = eOpen;
	m_Image.SetAsset("SelectWeapon");
	m_Pos = Vector2D(400.0f, 300.0f);
	m_Center = Vector2D(256.0f, 256.0f);
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
			if (m_Size < 512) m_Size += 32;
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
		if (m_Size > 0) m_Size -= 32;
		m_Image.SetSize(m_Size, m_Size);




	}
	m_Image.Draw(m_Pos.x, m_Pos.y, m_Center.x, m_Center.y);
}

EWeapons SelectWeapon::Select()
{
	m_isActive = !m_isActive;
	return m_Weapon;
}

void SelectWeapon::Draw()
{

}
