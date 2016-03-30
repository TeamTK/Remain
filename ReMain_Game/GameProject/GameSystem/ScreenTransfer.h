#ifndef _SCREEN_TRANSFER_H_
#define _SCREEN_TRANSFER_H_

#include "..\GEKO\GEKO.h"

//�t�F�[�h�C��
class ScreenTransfer_In
{
public:
	ScreenTransfer_In();
	~ScreenTransfer_In();
	bool GetIsEndTransfer();
	void Start(float TransferSpeed);
	void Update();

private:
	bool m_IsStart;
	float m_AlphaNum;
	float m_TransferSpeed;
	Image m_BlackImage;
};

//�t�F�[�h�A�E�g
class ScreenTransfer_Out
{
public:
	ScreenTransfer_Out();
	~ScreenTransfer_Out();
	bool GetIsEndTransfer();
	void Start(float TransferSpeed);
	void Update();

private:
	bool m_IsStart;
	float m_AlphaNum;
	float m_TransferSpeed;
	Image m_BlackImage;
};

#endif