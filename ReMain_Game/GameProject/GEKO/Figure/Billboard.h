#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "FiqureBase.h"

//�r���{�[�h�̒��_���
struct VertexBillboardInfo
{
	//���_���W
	Vector3D leftTopPos;	 //����̍��W
	Vector3D leftDwonPos;	 //�����̍��W
	Vector3D rightTopPos;	 //�E��̍��W
	Vector3D rightDwonPos;	 //�E���̍��W

	//�e�N�X�`��UV���W
	Vector2D leftTopUV;	 //����UV���W
	Vector2D leftDwonUV;	 //����UV���W
	Vector2D rightTopUV;	 //�E��UV���W
	Vector2D rightDwonUV;	 //�E��UV���W

	VertexBillboardInfo() :
		leftTopPos(-1.0, 1.0, 0.0f),
		leftDwonPos(-1.0, -1.0, 0.0f),
		rightTopPos(1.0, 1.0, 0.0f),
		rightDwonPos(1.0, -1.0, 0.0f),
		leftTopUV(0.0f, 0.0f),
		leftDwonUV(0.0f, 1.0f),
		rightTopUV(1.0f, 0.0f),
		rightDwonUV(1.0f, 1.0f)
	{
	}
};

//�r���{�[�h
class Billboard
{
public:
	Billboard();
	~Billboard();
	void ChangeVertex(const VertexBillboardInfo& info);
	void Render(const Vector3D &pos, float size, const std::string &name);

private:
	ID3D11Buffer* m_pVertexBuffer; //���_�o�b�t�@�[

};

//�r���{�[�h�A�j���[�V����
class BillboardAnimation
{
public:
	BillboardAnimation();
	BillboardAnimation(const std::string &assetName, int frameNum, int sizeW, int sizeH);
	~BillboardAnimation();
	bool GetIsEnd() const;
	void FrameDivision(const std::string &assetName, int frameNum, int sizeW, int sizeH);
	void PlayFrame(float frame);
	void Render(const Vector3D &pos, float size);
	void DebugFrame();

private:
	bool m_IsEnd;
	int m_FrameNum;
	int m_FrameAllNum;
	float m_Speed;
	Billboard *m_pBillboard;
	std::string m_Name;
};

#endif