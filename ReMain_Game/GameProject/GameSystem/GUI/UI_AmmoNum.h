#ifndef UI_AMMONUM_H
#define UI_AMMONUM_H

#include "../../GameSystem/GUI/Number.h"

class UI_AmmoNum : public Task
{
public:
	UI_AmmoNum();
	~UI_AmmoNum();
	void Update();

private:
	Number m_AmmoNum;
	Number m_LoadedAmmoNum;
	Image m_Slash;
};
#endif