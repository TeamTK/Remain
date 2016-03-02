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

ImageAnimation::ImageAnimation(const std::string &assetName, int frameNum, int sizeW, int sizeH) :
	m_pLeftX(nullptr),
	m_pLeftY(nullptr),
	m_pRightX(nullptr),
	m_pRightY(nullptr),
	m_Speed(0.0f)
{
	FrameDivision(assetName, frameNum, sizeW, sizeH);
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

void ImageAnimation::FrameDivision(const std::string &assetName, int frameNum, int sizeW, int sizeH)
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

	int width = m_Image.GetWidth();
	int height = m_Image.GetHeight();

	//幅縦の分割数カウント変数
	int DivW = width / sizeW;
	int DivH = height / sizeH;

	m_pLeftX = new int[m_FrameAllNum];
	m_pLeftY = new int[m_FrameAllNum];
	m_pRightX = new int[m_FrameAllNum];
	m_pRightY = new int[m_FrameAllNum];

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
		if (cntW == DivW)
		{
			cntW = 0;
			cntSizeW = 0;
			cntH++;
			cntSizeH += sizeH;
		}

		//縦方向の分割更新
		if (cntH > DivH)
		{
			cntH = 0;
			cntSizeH = 0;
			break;
		}
	}
	m_Image.SetSize(sizeW, sizeH);
}

void ImageAnimation::PlayFrame(float frame)
{
	m_Speed += frame;
}

void ImageAnimation::Draw(const Vector2D &pos)
{
	m_FrameNum = (int)m_Speed;

	//設定したフレームを超えたら初期化
	if (m_FrameNum >= m_FrameAllNum)
	{
		m_Speed = 0;
		m_FrameNum = 0;
		m_IsEnd = true;
	}

	m_Image.SetDrawPos(m_pLeftX[m_FrameNum], m_pLeftY[m_FrameNum], m_pRightX[m_FrameNum], m_pRightY[m_FrameNum]);
	m_Image.Draw(pos);
}

void ImageAnimation::DebugFrame()
{
	printf("%d\n", m_FrameNum);
}