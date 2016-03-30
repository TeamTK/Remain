#ifndef _UI_LIFE_H_
#define _UI_LIFE_H_

#include "../../GEKO/GEKO.h"
#include "../GUI/Number.h"

class UI_Life : public Task
{
public:
	UI_Life(float hpMax, float* pHp, int* pRecItemNum);
	~UI_Life();
	void Update();

private:
	int* m_RecoveryItemNum;
	float m_HpMax;
	float* m_pHp;
	Image m_LifeImage;
	Image m_LifeFrameImage;
	Image m_LifeBackImage;
	Number m_RecItemNumber;	//‰ñ•œ–ò‚ÌŒÂ”•\¦
};

#endif