#ifndef _TITLE_H_
#define _TITLE_H_

#include "..\GEKO\GEKO.h"
#include "..\GameSystem\ScreenTransfer.h"

class Title : public Task
{
public:
	Title();
	~Title();
	void Update();
	void Render();

private:
	enum Select
	{
		eNo,
		eStart,
		eExit
	};

	bool m_IsTransferStart;
	bool m_IsTransferEnd;
	RenderTask m_Render;
	Image m_TitleImage;
	Image m_StartImage;
	Image m_ExitImage;
	Image m_SelectImage;
	StaticMesh m_Cabin;
	StaticMesh m_ShotGun;
	StaticMesh m_HandGun;
	StaticMesh m_AmmoBox;
	Vector2D m_SelectPos;
	Select m_Select;
	ScreenTransfer_In m_Transfer;
	ScreenTransfer_Out m_Transfer_Out;
	Sound m_DecisionSound;
	Sound m_CursorSound;
};

#endif