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
	void Draw();

private:
	bool m_IsFadeOut;
	float m_Alpha;
	RenderTask m_Render;
	Image m_RecoveryImage;
};

#endif