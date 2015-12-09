#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "ImageAsset.h"

//���_�̍\����
struct Vertex
{
	D3DXVECTOR3 Pos; //�ʒu
	D3DXVECTOR2 UV; //�e�N�X�`���[���W
};

struct RGBA
{
	float sRed;
	float sGreen;
	float sBlue;
	float sAlpha;
};

class Image
{
public:
	Image();
	~Image();
	int GetWidth();
	int GetHeight();
	void SetAsset(const char *name);
	void SetAlpha(int alpha);
	void SetAngle(int angle);
	void SetRGB(int red, int green, int blue);
	void SetSize(int w, int h);
	//void SetCenter(int x, int y);
	void SetDrawPos(int x, int y, int width, int height);
	void Draw(int x, int y, int centerX = 0, int centerY = 0);

private:
	void InitModel(int centerX, int centerY);

private:
	ImageData *m_pImageData;

	RECT m_UvSize;  //UV���W�ɕϊ��O�̃s�N�Z���l
	RGBA m_RGBA;	//RGBA��ݒ�

	int m_SizeW;	//�摜�̕�
	int m_SizeH;	//�摜�̍���
	int m_Angle;    //�摜�̊p�x�i�x���j
};

#endif