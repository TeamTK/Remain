#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "FiqureBase.h"
#include "../Shader/RenderingManager.h"
#include "../Mesh/MeshInfo.h"

struct ImageInfo;

//�r���{�[�h�̒��_���
struct VertexBillboardInfo
{
	//���_���W
	Vector3D leftTopPos;	 //����̍��W
	Vector3D leftDwonPos;	 //�����̍��W
	Vector3D rightTopPos;	 //�E��̍��W
	Vector3D rightDwonPos;	 //�E���̍��W

	//�e�N�X�`��UV���W
	Vector2D leftTopUV;		 //����UV���W
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
	Billboard(const std::string &assetName, unsigned int priorityGroup, unsigned int priority);
	~Billboard();
	void SetRenderingRegister(bool isRegister, unsigned int priorityGroup, unsigned int priority);
	void SetRenderingType(RenderingType renderingType);
	void SetPriority(unsigned int priorityGroup, unsigned int priority);
	void SetImageAsset(const std::string &assetName);
	void SetPosition(float x, float y);
	void SetPosition(const Vector3D &position);
	void SetColor(int r, int g, int b);
	void SetAlpha(int alpha);
	void SetSize(float size);

private:
	void InitVertex(const VertexBillboardInfo& info);
	void Render();

private:
	bool m_IsRenderingRegister;    //�����_�����O�o�^���f
	float m_Size;				   //�r���{�[�h�̑傫��
	ID3D11Buffer* m_pVertexBuffer; //���_�o�b�t�@�[
	ImageInfo *m_pImageInfo;	   //�摜���ւ̃|�C���^
	RenderingType m_RenderingType; //�����_�����O�̎��
	RenderingInfo m_Rendering;	   //�����_�����O�Ǘ����
	Vector3D m_Position;		   //�r���{�[�h�̍��W
	Vector4D m_Color;			   //�r���{�[�h�̐F
};

//�r���{�[�h�A�j���[�V����
class BillboardAnimation
{
public:
	BillboardAnimation();
	BillboardAnimation(const std::string &assetName, unsigned int priorityGroup, unsigned int priority, int totalFrameNum, int divW, int divH);
	~BillboardAnimation();
	bool GetIsEnd() const;
	void SetRenderingRegister(bool isRegister, unsigned int priorityGroup, unsigned int priority);
	void SetRenderingType(RenderingType renderingType);
	void SetPriority(unsigned int priorityGroup, unsigned int priority);
	void SetPosition(float x, float y);
	void SetPosition(const Vector3D &position);
	void SetColor(int r, int g, int b);
	void SetAlpha(int alpha);
	void SetSize(float size);
	void FrameDivision(const std::string &assetName, int totalFrameNum, int divW, int divH);
	void PlayFrame(float frame);
	void DebugFrame();

private:
	void InitVertex(const VertexBillboardInfo& info);
	void Render();

private:
	bool m_IsRenderingRegister;    //�����_�����O�o�^���f
	bool m_IsEnd;				   //�A�j���[�V�����̏I��蔻�f
	int m_CurrentFrameNum;		   //���݂̃A�j���[�V�����t���[���ԍ�
	int m_PastFrameNum;			   //�ߋ��̃A�j���[�V�����t���[���ԍ�
	int m_TotalFrameNum;		   //�A�j���[�V�����t���[���̍��v
	float m_Speed;				   //�A�j���[�V�������x
	float m_Size;				   //�r���{�[�h�̑傫��
	ID3D11Buffer *m_pVertexBuffer; //���_�o�b�t�@�[
	ImageInfo *m_pImageInfo;       //�摜���ւ̃|�C���^
	RenderingType m_RenderingType; //�����_�����O�̎��
	RenderingInfo m_Rendering;	   //�����_�����O�Ǘ����
	Vector2D *m_pLeftTopUV;		   //����UV���W
	Vector2D *m_pLeftDwonUV;	   //����UV���W
	Vector2D *m_pRightTopUV;	   //�E��UV���W
	Vector2D *m_pRightDwonUV;	   //�E��UV���W
	Vector3D m_Position;		   //�r���{�[�h�̍��W
	Vector4D m_Color;			   //�r���{�[�h�̐F
};

#endif