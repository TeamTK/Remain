#ifndef UI_SELECT_WEAPON_H
#define UI_SELECT_WEAPON_H

#include "../../MainGame/enums.h"
#include "../../GEKO/GEKO.h"

class UI_SelectWeapon
{
public:
	UI_SelectWeapon();
	~UI_SelectWeapon();
	void Update();
	void Draw();
	bool GetisSelected();
	EWeapons Select();

private:
	Image m_Circle;
	Image m_SelectedCursor;
	Image m_WeaponUI[4];
	Vector2D m_ScPos;	//カーソルの座標
	Vector2D m_UIPos[4];
	RenderTask m_RenderTask;
	int m_CircleSize;
	int m_WeponUISize;
	int m_Selected;
	bool m_isSelected;
};
#endif