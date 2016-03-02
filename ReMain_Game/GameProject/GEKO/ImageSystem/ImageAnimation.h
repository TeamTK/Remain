#ifndef _IMAGE_ANIMATION_H_
#define _IMAGE_ANIMATION_H_

#include "Image.h"

class ImageAnimation
{
public:
	ImageAnimation();
	ImageAnimation(const std::string &assetName, int frameNum, int sizeW, int sizeH);
	~ImageAnimation();
	bool GetIsEnd() const;
	void FrameDivision(const std::string &assetName, int frameNum, int sizeW, int sizeH);
	void PlayFrame(float frame);
	void Draw(const Vector2D &pos);
	void DebugFrame();

private:
	bool m_IsEnd;
	int m_FrameNum;
	int m_FrameAllNum;
	int *m_pLeftX;
	int *m_pLeftY;
	int *m_pRightX;
	int *m_pRightY;
	float m_Speed;
	Image m_Image;
};

#endif