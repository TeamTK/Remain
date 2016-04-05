#ifndef _MESH_BASE_H_
#define _MESH_BASE_H_

#include "StaticMesh\StaticMeshAsset.h"
#include "WorldMatrixManager.h"
#include "../Shader/RenderingManager.h"

struct ImageInfo;

//StaticMesh�EDynamicMesh�^�̊�ꃁ�b�V����̋��ʍ��ڂ��`
class MeshBase
{
public:
	MeshBase(bool isRegister, unsigned int priorityGroup, unsigned int priority, unsigned int meshState);
	virtual ~MeshBase();
	void SetRenderingRegister(bool isRegister, unsigned int priorityGroup, unsigned int priority);
	void SetRenderingType(RenderingType renderingType);
	void SetPriority(unsigned int priorityGroup, unsigned int priority);
	void SetScale(float x, float y, float z);
	void SetRotationRadian(float x, float y, float z);
	void SetRotationDegree(int x, int y, int z);
	void SetTranselate(float x, float y, float z);
	void SetTranselate(const Vector3D &pos);
	void SetDiffuse(int r, int g, int b, int materialIndex);
	void SetSpecular(int r, int g, int b, int materialIndex);
	void SetAmbient(int r, int g, int b, int materialIndex);
	void SetAlpha(int alpha, int materialIndex);
	void SetAlphaAll(int alpha);
	void SetTexture(const std::string &assetName, bool isTexture = true);
	void SetModelMatrixBuilding();
	void SetParentMatirx(const Matrix &parentMatrix);
	Vector3D GetScale() const;
	Vector3D GetRotation() const;
	Vector3D GetTranselate() const;
	Vector3D GetDiffuse(int materialIndex) const;
	Vector3D GetSpecular(int materialIndex) const;
	Vector3D GetAmbient(int materialIndex) const;
	Vector3D GetAxisX(MatrixType matrixType) const;
	Vector3D GetAxisY(MatrixType matrixType) const;
	Vector3D GetAxisZ(MatrixType matrixType) const;
	const Matrix *GetLocalMatrix() const;
	const Matrix *GetWorldMatrix() const;
	const Matrix *GetModelMatrix() const;
	void DebugAxis(MatrixType matrixType);

protected:
	virtual void ForwardRendering();
	virtual void DeferredRendering();

protected:
	bool m_IsRenderingRegister;	   //�����_�����O�o�^���f
	unsigned int m_MeshState;		   //���f���̏�Ԃ𔻒f
	Matrix *m_pLocalMatrix;		   //���f���̃��[�J���s��ւ̃|�C���^
	Matrix m_WorldMatrix;			   //���f���̃��[���h�s��
	Matrix m_ModelMatrix;		       //���f���̃��[�J�� �~ ���[���h�s��
	const Matrix *m_pParentMatirx;	   //�e�̃��f���s��
	Vector3D m_Rotation;			   //���f���̉�]�l�iX���EY���EZ���j
	Vector3D m_Scale;				   //���f���̊g�嗦�iX���EY���EZ���j
	Vector3D m_Transelate;			   //���f���̕��s�ړ��iX���EY���EZ���j
	WorldMatrixInfo m_WorldMatrixInfo; //���f���̉�]�E�g��E���s�ړ��𑼂̏ꏊ�Ōv�Z���邽�߂̏��
	std::vector<Vector4D> m_Diffuse;   //���f���̊g�U���˗�
	std::vector<Vector3D> m_Specular;  //���f���̋��ʔ��˗�
	std::vector<Vector3D> m_Ambient;   //���f���̊�����
	ImageInfo *m_pImageInfo;		   //�}���`�e�N�X�`���K�p�̉摜���ւ̃|�C���^
	RenderingType m_RenderingType;	   //�����_�����O�̎��
	RenderingInfo m_RenderingInfo;	   //�����_�����O�Ǘ����
};

#endif