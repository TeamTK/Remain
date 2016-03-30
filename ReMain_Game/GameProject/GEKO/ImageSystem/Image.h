#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "ImageAsset.h"
#include "../Shader/RenderingManager.h"

class StaticMesh;
class DynamicMesh;

//���_�̍\����
struct Vertex
{
	Vector3D Pos; //�ʒu
	Vector2D UV; //�e�N�X�`���[���W
};

struct RGBA
{
	float sRed;
	float sGreen;
	float sBlue;
	float sAlpha;
};

//�摜��`��
class Image
{ 
	friend StaticMesh;
	friend DynamicMesh;

public:
	Image();
	Image(const std::string &assetName, unsigned int priorityGroup, unsigned int priority);
	~Image();
	int GetWidth() const;
	int GetHeight() const;
	void SetDrawRegister(bool isRegister, unsigned int priorityGroup, unsigned int priority);
	void SetAsset(const std::string &assetName);
	void SetDrawPriority(unsigned int priorityGroup, unsigned int priority);
	void SetAlpha(int alpha);
	void SetAngle(int angle);
	void SetRGB(int red, int green, int blue);
	void SetSize(int w, int h);
	void SetCenter(int x, int y);
	void SetDrawArea(int leftX, int leftY, int rightX, int rightY);
	void SetPosition(float x, float y);
	void SetPosition(const Vector2D &position);

private:
	void InitModel(int centerX, int centerY);
	void Draw();

private:
	bool m_IsDrawRegister; //�`��o�^���f
	int m_SizeW;	//�摜�̕�
	int m_SizeH;	//�摜�̍���
	int m_Angle;    //�摜�̊p�x�i�x���j
	int m_CenterX;	//�摜�̒��SX
	int m_CenterY;	//�摜�̒��SY
	Vector2D m_Pos; //�摜�ʒu�i���オ���_�̃X�N���[�����W�j
	ImageData *m_pImageData; //�摜�f�[�^�󂯎��p
	RECT m_UvSize;  //UV���W�ɕϊ��O�̃s�N�Z���l
	RGBA m_RGBA;	//RGBA��ݒ�
	RenderingInfo m_RenderingInfo; //�����_�����O�Ǘ�
};

#endif