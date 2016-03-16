#ifndef _UI_LIFE_H_
#define _UI_LIFE_H_

#include "../../GEKO/GEKO.h"

class UI_Life
{
public:
	UI_Life();
	~UI_Life();
	void SetIsHPDraw(bool isDraw);
	void SetHp(float hp);
	void Update();

private:
	void Render();

private:
	float m_hp;
	RenderTask m_Render;
	Image m_LifeImage;
};

#endif