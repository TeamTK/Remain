#ifndef _SCREEN_TRANSFER_H_
#define _SCREEN_TRANSFER_H_

#include "..\GEKO\ImageSystem\Image.h"

//フェードイン
class ScreenTransfer_In
{
public:
	ScreenTransfer_In();
	~ScreenTransfer_In();
	bool GetIsEndTransfer();
	void Start(float TransferSpeed);
	void Update();
	void Render();

private:
	bool m_IsStart;
	int m_AlphaNum;
	float m_AlphaCnt;
	float m_TransferSpeed;
	Image m_BlackImage;
};

//フェードアウト
class ScreenTransfer_Out
{
public:
	ScreenTransfer_Out();
	~ScreenTransfer_Out();
	bool GetIsEndTransfer();
	void Start(float TransferSpeed);
	void Update();
	void Render();

private:
	bool m_IsStart;
	float m_AlphaCnt;
	float m_TransferSpeed;
	Image m_BlackImage;
};

#endif