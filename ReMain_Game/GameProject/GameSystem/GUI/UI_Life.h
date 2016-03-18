#ifndef _UI_LIFE_H_
#define _UI_LIFE_H_

#include "../../GEKO/GEKO.h"

class UI_Life : public Task
{
public:
	UI_Life(float hpMax, float* pHp);
	~UI_Life();
	void Update();

private:
	void Render();

private:
	float m_HpMax;
	float* m_pHp;
	RenderTask m_Render;
	Image m_LifeImage;
	Image m_LifeFrameImage;
	Image m_LifeBackImage;
};

#endif