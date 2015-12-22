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
	EWeapons m_Weapon;
	Vector2D m_Pos;
	int m_Size;
	bool m_isActive;
};
#endif