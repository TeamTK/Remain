#include "ScreenTransfer.h"

ScreenTransfer_In::ScreenTransfer_In() :
	m_IsStart(false),
	m_AlphaNum(0),
	m_AlphaCnt(0.0f),
	m_TransferSpeed(0.0f)
{
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
		m_AlphaCnt = 0.0f;
		m_AlphaNum = 0;
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
		m_BlackImage.SetAlpha(m_AlphaNum);
		if (m_AlphaCnt >= 60.0f)
		{
			m_AlphaNum++;
			m_AlphaCnt = 0.0f;
			std::cout << m_AlphaNum << "\n";
		}
		m_AlphaCnt += m_TransferSpeed;
	}
}

void ScreenTransfer_In::Render()
{
	m_BlackImage.Draw(0, 0);
}

ScreenTransfer_Out::ScreenTransfer_Out() :
	m_IsStart(false),
	m_AlphaCnt(255.0f)
{
	m_BlackImage.SetAsset("Transfer_Black");
	m_BlackImage.SetAlpha(255);
}

ScreenTransfer_Out::~ScreenTransfer_Out()
{
}

bool ScreenTransfer_Out::GetIsEndTransfer()
{
	if (m_AlphaCnt < 0)
	{
		m_IsStart = false;
		m_AlphaCnt = 255.0f;
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
		m_BlackImage.SetAlpha((int)m_AlphaCnt);
		m_AlphaCnt -= m_TransferSpeed;
	}
}

void ScreenTransfer_Out::Render()
{
	m_BlackImage.Draw(0, 0);
}