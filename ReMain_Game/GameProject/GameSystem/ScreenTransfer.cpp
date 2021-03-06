#include "ScreenTransfer.h"

ScreenTransfer_In::ScreenTransfer_In() :
	m_IsStart(false),
	m_AlphaNum(0.0f),
	m_TransferSpeed(0.0f)
{
	m_BlackImage.SetDrawRegister(true, 10, 0);
	m_BlackImage.SetAsset("Transfer_Black");
	m_BlackImage.SetAlpha(0);
}

ScreenTransfer_In::~ScreenTransfer_In()
{
}

bool ScreenTransfer_In::GetIsEndTransfer()
{
	if (m_AlphaNum > 255)
	{
		m_IsStart = false;
		return true;
	}
	return false;
}

void ScreenTransfer_In::Start(float TransferSpeed)
{
	m_TransferSpeed = TransferSpeed;
	m_IsStart = true;
}

void ScreenTransfer_In::Update()
{
	if (m_IsStart)
	{
		m_AlphaNum += m_TransferSpeed;
		m_BlackImage.SetAlpha((int)m_AlphaNum);
	}
}

ScreenTransfer_Out::ScreenTransfer_Out() :
	m_IsStart(false),
	m_AlphaNum(255.0f)
{
	m_BlackImage.SetDrawRegister(true, 10, 0);
	m_BlackImage.SetAsset("Transfer_Black");
	m_BlackImage.SetAlpha(255);
}

ScreenTransfer_Out::~ScreenTransfer_Out()
{
}

bool ScreenTransfer_Out::GetIsEndTransfer()
{
	if (m_AlphaNum < 0)
	{
		m_IsStart = false;
		return true;
	}
	return false;
}

void ScreenTransfer_Out::Start(float TransferSpeed)
{
	m_TransferSpeed = TransferSpeed;
	m_IsStart = true;
}

void ScreenTransfer_Out::Update()
{
	if (m_IsStart)
	{
		m_BlackImage.SetAlpha((int)m_AlphaNum);
		m_AlphaNum -= m_TransferSpeed;
	}
}