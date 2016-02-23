#ifndef UI_RETICLE_H
#define UI_RETICLE_H

#include "../../GEKO/GEKO.h"

class UI_Reticle : public Task
{
public:
	UI_Reticle(bool* setupGun);
	~UI_Reticle();
	void Update();
	void Draw();

private:
	Image m_Reticle;
	RenderTask m_RenderTask;
	bool m_isSetupWeapon;	//プレイヤーが武器を構えているか
};
#endif