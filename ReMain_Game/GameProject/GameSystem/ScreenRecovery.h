#ifndef _SCREEN_RECOVERY_H_
#define _SCREEN_RECOVERY_H_

#include "../GEKO/GEKO.h"

class ScreenRecovery : public Task
{
public:
	ScreenRecovery();
	~ScreenRecovery();
	void Update();

private:
	bool m_IsFadeOut;
	float m_Alpha;
	Image m_RecoveryImage;
};

#endif