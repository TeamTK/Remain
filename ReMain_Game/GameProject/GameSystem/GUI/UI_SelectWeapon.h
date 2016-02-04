#ifndef UI_SELECT_WEAPON_H
#define UI_SELECT_WEAPON_H

#include "../../MainGame/Player/PlayerInfo.h"
#include "../../MainGame/Weapon/WeaponInfo.h"

class UI_SelectWeapon
{
public:
	UI_SelectWeapon();
	~UI_SelectWeapon();
	void Update();
	void Draw();
	bool isSelected();
	EWeapons Select();

private:
	Image m_Circle;
	Image m_SelectedCursor;
	Image m_WeaponUI[4];
	Vector3D m_ScPos;	//カーソルの座標 XYのみ使用
	Vector3D m_UIPos[4];	//各武器UI座標 XYのみ使用
	RenderTask m_RenderTask;
	int m_CircleSize;
	int m_WeponUISize;
	int m_Selected;
	bool m_isSelected;
};
#endif