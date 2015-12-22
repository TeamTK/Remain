#ifndef SELECT_WEAPON_H
#define SELECT_WEAPON_H

#include "../../MainGame/enums.h"
#include "../../GEKO/GEKO.h"

class SelectWeapon
{
public:
	SelectWeapon();
	~SelectWeapon();
	void Update();
	void Draw();
	EWeapons Select();


private:
	Image m_Image;
	Image m_SelectedCircle;
	EWeapons m_Weapon;
	Vector2D m_Pos;
	Vector2D m_ScPos;
	int m_Size;
	int m_Selected;
	bool m_isActive;
};
#endif