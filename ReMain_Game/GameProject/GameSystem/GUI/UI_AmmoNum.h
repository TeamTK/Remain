#ifndef UI_AMMONUM_H
#define UI_AMMONUM_H

#include "../../GameSystem/GUI/Number.h"

class UI_AmmoNum : public Task
{
public:
	UI_AmmoNum();
	~UI_AmmoNum();
	void Update();
	void Draw();

private:
	Number m_Num;
	Image m_Slash;
	RenderTask m_RenderTask;
};
#endif