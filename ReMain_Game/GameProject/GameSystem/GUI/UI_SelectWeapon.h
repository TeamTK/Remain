#ifndef UI_SELECT_WEAPON_H
#define UI_SELECT_WEAPON_H

#include "../../MainGame/enums.h"
#include "../../GEKO/GEKO.h"

class UI_SelectWeapon : public Task
{
public:
	UI_SelectWeapon();
	~UI_SelectWeapon();
	void Update();
	void Draw();
	bool GetisSelected();
	EWeapons Select();

private:
	Image m_Image;
	Image m_SelectedCursor;
	EWeapons m_Weapon;	//����̎��
	Vector2D m_ScPos;	//�J�[�\���̍��W	
	RenderTask m_RenderTask;
	int m_Size;
	int m_Selected;
	bool m_isSelected;
};
#endif