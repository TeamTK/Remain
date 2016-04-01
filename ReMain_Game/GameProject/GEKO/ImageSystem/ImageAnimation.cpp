#include "ImageAnimation.h"

ImageAnimation::ImageAnimation() :
	m_IsEnd(false),
	m_pLeftX(nullptr),
	m_pLeftY(nullptr),
	m_pRightX(nullptr),
	m_pRightY(nullptr),
	m_Speed(0.0f)
{
}

ImageAnimation::ImageAnimation(const std::string &assetName, unsigned int priorityGroup, unsigned int priority, int frameNum, int divW, int divH) :
	m_pLeftX(nullptr),
	m_pLeftY(nullptr),
	m_pRightX(nullptr),
	m_pRightY(nullptr),
	m_Speed(0.0f)
{
	m_Image.SetDrawRegister(true, priorityGroup, priority);
	FrameDivision(assetName, frameNum, divW, divH);
}

ImageAnimation::~ImageAnimation()
{
	if (m_pLeftX != nullptr)
	{
		delete[] m_pLeftX;
		delete[] m_pLeftY;
		delete[] m_pRightX;
		delete[] m_pRightY;
	}
}

bool ImageAnimation::GetIsEnd() const
{
	return m_IsEnd;
}

void ImageAnimation::SetDrawRegister(bool isRegister, unsigned int priorityGroup, unsigned int priority)
{
	m_Image.SetDrawRegister(isRegister, priorityGroup, priority);
}

void ImageAnimation::FrameDivision(const std::string &assetName, int frameNum, int divW, int divH)
{
	m_Image.SetAsset(assetName);

	m_FrameAllNum = frameNum;
	m_FrameNum = 0;
	m_Speed = 0.0f;
	m_IsEnd = false;

	if (m_pLeftX != nullptr)
	{
		delete[] m_pLeftX;
		delete[] m_pLeftY;
		delete[] m_pRightX;
		delete[] m_pRightY;
	}
	m_pLeftX = new int[m_FrameAllNum];
	m_pLeftY = new int[m_FrameAllNum];
	m_pRightX = new int[m_FrameAllNum];
	m_pRightY = new int[m_FrameAllNum];

	int width = m_Image.GetWidth();
	int height = m_Image.GetHeight();

	//幅縦の分割数カウント変数
	int sizeW = 0;
	int sizeH = 0;

	if (divW == 0) sizeW = width / 1;
	else		   sizeW = width / divW;

	if (divH == 0) sizeH = height / 1;
	else		   sizeH = height / divH;

	//幅縦の分割数カウント変数
	int cntW = 0;
	int cntH = 0;

	//画像分割幅と高さ
	int cntSizeW = 0;
	int cntSizeH = 0;

	for (int i = 0; i < m_FrameAllNum; i++)
	{
		m_pLeftX[i] = cntSizeW;
		m_pRightX[i] = (cntSizeW + sizeW);
		m_pLeftY[i] = cntSizeH;
		m_pRightY[i] = (cntSizeH + sizeH);

		//幅は右に沿って更新
		cntW++;
		cntSizeW += sizeW;

		//横方向の分割更新
		if (cntW == divW)
		{
			cntW = 0;
			cntSizeW = 0;
			cntH++;
			cntSizeH += sizeH;
		}

		//縦方向の分割更新
		if (cntH > divH)
		{
			cntH = 0;
			cntSizeH = 0;
			break;
		}
	}
	m_Image.SetSize(sizeW, sizeH);
	m_Image.SetDrawArea(m_pLeftX[0], m_pLeftY[0], m_pRightX[0], m_pRightY[0]);
}

void ImageAnimation::PlayFrame(float frame)
{
	m_Speed += frame;
}

void ImageAnimation::SetPosition(float x, float y)
{
	m_Image.SetPosition(x, y);
}

void ImageAnimation::SetPosition(const Vector2D &positio)
{
	m_Image.SetPosition(positio);
}

void ImageAnimation::SetDrawPriority(unsigned int priorityGroup, unsigned int priority)
{
	m_Image.SetDrawPriority(priorityGroup, priority);
}

void ImageAnimation::Update()
{
	m_FrameNum = (int)m_Speed;

	//設定したフレームを超えたら初期化
	if (m_FrameNum >= m_FrameAllNum)
	{
		m_Speed = 0;
		m_FrameNum = 0;
		m_IsEnd = true;
	}

	m_Image.SetDrawArea(m_pLeftX[m_FrameNum], m_pLeftY[m_FrameNum], m_pRightX[m_FrameNum], m_pRightY[m_FrameNum]);
}

void ImageAnimation::DebugFrame()
{
	printf("%d\n", m_FrameNum);
}