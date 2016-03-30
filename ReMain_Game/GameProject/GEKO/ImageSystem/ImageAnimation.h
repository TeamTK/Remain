#ifndef _IMAGE_ANIMATION_H_
#define _IMAGE_ANIMATION_H_

#include "Image.h"

class ImageAnimation
{
public:
	ImageAnimation();
	ImageAnimation(const std::string &assetName, unsigned int priorityGroup, unsigned int priority, int frameNum, int divW, int divH);
	~ImageAnimation();
	bool GetIsEnd() const;
	void SetDrawRegister(bool isRegister, unsigned int priorityGroup, unsigned int priority);
	void FrameDivision(const std::string &assetName, int frameNum, int divW, int divH);
	void PlayFrame(float frame);
	void SetPosition(float x, float y);
	void SetPosition(const Vector2D &position);
	void SetDrawPriority(unsigned int priorityGroup, unsigned int priority);
	void Update();
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