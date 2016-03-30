#ifndef UI_SELECT_WEAPON_H
#define UI_SELECT_WEAPON_H

#include "../../MainGame/Player/PlayerInfo.h"
#include "../../MainGame/Weapon/WeaponInfo.h"

class UI_SelectWeapon : public Task
{
public:
	UI_SelectWeapon();
	~UI_SelectWeapon();
	void Update();
	bool isSelected();
	bool HitCheck(Vector3D pos1, Vector3D pos2, float radius1, float radius2);
	EWeapons Select();
	EWeapons GetSelect();

private:
	Image m_Circle;
	Image m_SelectedCursor;
	Image m_WeaponUI[4];
	Sound m_CursorSound;
	Vector3D m_ScPos;	//カーソルの座標 XYのみ使用
	Vector3D m_UIPos[4];	//各武器UI座標 XYのみ使用
	int m_CircleSize;
	int m_WeponUISize;
	int m_Selected;
	int m_OldSelected;
	bool m_isSelected;
	bool m_isOldSelected;
};
extern UI_SelectWeapon* g_pUI_SelectWeapon;
#endif