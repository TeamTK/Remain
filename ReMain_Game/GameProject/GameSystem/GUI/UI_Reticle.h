#ifndef UI_RETICLE_H
#define UI_RETICLE_H

#include "../../GEKO/GEKO.h"

class UI_Reticle : public Task
{
public:
	UI_Reticle(bool* setupGun);
	~UI_Reticle();
	void Update();

private:
	Image m_Reticle;
	bool* m_isSetupWeapon;	//�v���C���[��������\���Ă��邩
	bool m_isOldSetupWeapon;
};
#endif